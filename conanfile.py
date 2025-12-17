import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy

class VoxelEngine(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    
    def requirements(self):
        self.requires("glad/2.0.8")
        self.requires("spdlog/1.16.0")
        self.requires("glm/1.0.1")
        self.requires("imgui/1.92.5")
        self.requires("joltphysics/5.2.0")
       
    def build_requirements(self):
        self.tool_requires("cmake/[>=3.25]")
        
    def generate(self):
        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
            "src", "bindings"), os.path.join(self.source_folder, "bindings"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
            "src", "bindings"), os.path.join(self.source_folder, "bindings"))
    
    def layout(self):
        cmake_layout(self)