import os

from conans import ConanFile, CMake, tools


class ReportportalclientcppTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_paths", "cmake_find_package"
    requires = "catch2/2.9.2", "fakeit/2.0.9"

    def build(self):
        cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
        # in "test_package"
        cmake.configure(source_folder="../../tests")
        cmake.build()

    def imports(self):
        pass

    def test(self):
        if not tools.cross_building(self.settings):
            cmake = CMake(self)
            # [TODO]: look into why parallel causes issues
            cmake.parallel = False
            cmake.test(output_on_failure=True)
