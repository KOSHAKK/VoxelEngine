#pragma once

#include <deque>


#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>




class PhysicsEngine 
{
public:
	struct Obj_settings
	{
		JPH::RVec3 pos;
		JPH::Vec3 size;
		JPH::EMotionType motion_type;
	};


	static void init();
	static void terminate();

	static JPH::BodyID* add_object(Obj_settings settings);




public:
    static inline JPH::PhysicsSystem physics_system;
    static inline JPH::BodyInterface* body_interface = nullptr;
    static inline JPH::JobSystemThreadPool* job_system;

	static inline std::deque<JPH::BodyID> m_objects_id;

    //static inline JPH::BodyID cube_id;
    //static inline JPH::BodyID floor_id;
};