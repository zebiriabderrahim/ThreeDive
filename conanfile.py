import os
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import copy


class ThreeDive(ConanFile):
    name = "ThreeDive"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        requirements = self.conan_data.get('requirements', [])
        for requirement in requirements:
            self.requires(requirement)

    def generate(self):
        for binding in ["*glfw*", "*opengl3*"]:
            copy(self, binding,
                 os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
                 os.path.join(self.source_folder, "bindings"))

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
