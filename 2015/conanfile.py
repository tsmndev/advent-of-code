import os

from conan import ConanFile


class adventOfCode2015Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        pass

    def layout(self):
        self.folders.generators = os.path.join("build", str(self.settings.build_type), "generators")
        self.folders.build = os.path.join("build", str(self.settings.build_type))
