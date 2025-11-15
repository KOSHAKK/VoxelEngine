#include "PhysicsEngine.hpp"

#include <iostream>
#include <common/Log.hpp>
#include <stdarg.h>


using namespace JPH::literals;

namespace {

	static void TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);
		// Print to the TTY
		std::cout << buffer << std::endl;
	}

#ifdef JPH_ENABLE_ASSERTS
	// Callback for asserts, connect this to your own assert handler if you have one
	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// Print to the TTY
		std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

		// Breakpoint
		return true;
	};
#endif // JPH_ENABLE_ASSERTS



	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};

	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
		{
			switch (inObject1)
			{
			case Layers::NON_MOVING:
				return inObject2 == Layers::MOVING; // Non moving only collides with moving
			case Layers::MOVING:
				return true; // Moving collides with everything
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr JPH::uint NUM_LAYERS(2);
	};

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			// Create a mapping table from object to broad phase layer
			mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
			mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
		}

		virtual JPH::uint					GetNumBroadPhaseLayers() const override
		{
			return BroadPhaseLayers::NUM_LAYERS;
		}

		virtual JPH::BroadPhaseLayer			GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
		{
			JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
			return mObjectToBroadPhase[inLayer];
		}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
		{
			switch ((JPH::BroadPhaseLayer::Type)inLayer)
			{
			case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
			case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
			default:													JPH_ASSERT(false); return "INVALID";
			}
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool				ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
		{
			switch (inLayer1)
			{
			case Layers::NON_MOVING:
				return inLayer2 == BroadPhaseLayers::MOVING;
			case Layers::MOVING:
				return true;
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	// An example contact listener
	class MyContactListener : public JPH::ContactListener
	{
	public:
		// See: ContactListener
		virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
		{
			std::cout << "Contact validate callback" << std::endl;

			// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
			return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
		}

		virtual void			OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
		{
			std::cout << "A contact was added" << std::endl;
		}

		virtual void			OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
		{
			std::cout << "A contact was persisted" << std::endl;
		}

		virtual void			OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
		{
			std::cout << "A contact was removed" << std::endl;
		}
	};

	// An example activation listener
	class MyBodyActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void		OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
		{
			std::cout << "A body got activated" << std::endl;
		}

		virtual void		OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
		{
			std::cout << "A body went to sleep" << std::endl;
		}
	};

}

void PhysicsEngine::init()
{
	JPH::RegisterDefaultAllocator();
	JPH::Trace = TraceImpl;

#ifdef NDEBUG
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;);
#else
	JPH::JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)
#endif

	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();

	const JPH::uint cMaxBodies = 1024;
	const JPH::uint cNumBodyMutexes = 0;
	const JPH::uint cMaxBodyPairs = 1024;
	const JPH::uint cMaxContactConstraints = 1024;

	static BPLayerInterfaceImpl            broad_phase_layer_interface;
	static ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
	static ObjectLayerPairFilterImpl       object_vs_object_layer_filter;

	physics_system.Init(
		cMaxBodies,
		cNumBodyMutexes,
		cMaxBodyPairs,
		cMaxContactConstraints,
		broad_phase_layer_interface,
		object_vs_broadphase_layer_filter,
		object_vs_object_layer_filter
	);

	// job system и temp allocator Ц живут столько же, сколько PhysicsEngine
	job_system = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers,
		std::thread::hardware_concurrency() - 1);
	body_interface = &physics_system.GetBodyInterface();

	// ---- создаЄм пол ----
	JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded();
	JPH::ShapeRefC floor_shape = floor_shape_settings.Create().Get();

	JPH::BodyCreationSettings floor_settings(
		floor_shape,
		JPH::RVec3(0.0_r, -1.0_r, 0.0_r),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		Layers::NON_MOVING
	);

	JPH::Body* floor = body_interface->CreateBody(floor_settings);
	floor_id = floor->GetID();
	body_interface->AddBody(floor_id, JPH::EActivation::DontActivate);

	// ---- создаЄм куб ----
	JPH::BoxShapeSettings cube_shape_settings(JPH::Vec3(2.0f, 2.0f, 2.0f));
	cube_shape_settings.SetEmbedded();
	JPH::ShapeRefC cube_shape = cube_shape_settings.Create().Get();

	JPH::BodyCreationSettings cube_settings(
		cube_shape,
		JPH::RVec3(0.0_r, 8.0_r, 0.0_r),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Dynamic,
		Layers::MOVING
	);

	cube_id = body_interface->CreateAndAddBody(cube_settings, JPH::EActivation::Activate);
	//body_interface->SetLinearVelocity(cube_id, JPH::Vec3(0.0f, -5.0f, 0.0f));

	physics_system.OptimizeBroadPhase();
}


void PhysicsEngine::terminate()
{
	
}