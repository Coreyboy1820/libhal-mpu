# Copyright 2024 Khalil Estell
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from conan import ConanFile

required_conan_version = ">=2.0.14"


class libhal_mpu_conan(ConanFile):
    name = "libhal-mpu"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://github.com/libhal/libhal-mpu"
    description = ("A collection of drivers for the mpu")
    topics = ("mpu", "IMU", "accelerometer")
    settings = "compiler", "build_type", "os", "arch"

    python_requires = "libhal-bootstrap/[^0.0.7]"
    python_requires_extend = "libhal-bootstrap.library"

    def requirements(self):
<<<<<<< HEAD
        self.requires("libhal/[^2.2.0]")
        self.requires("libhal-util/[^3.0.1]")

    def layout(self):
        cmake_layout(self)

    def build(self):
        run_test = not self.conf.get("tools.build:skip_test", default=False)

        cmake = CMake(self)
        if self.settings.os == "Windows":
            cmake.configure()
        elif self._bare_metal:
            cmake.configure(variables={
                "BUILD_TESTING": "OFF"
            })
        else:
            cmake.configure(variables={"ENABLE_ASAN": True})

        cmake.build()

        if run_test and not self._bare_metal:
            test_folder = os.path.join("tests")
            self.run(os.path.join(test_folder, "unit_test"))

    def package(self):
        copy(self,
             "LICENSE",
             dst=os.path.join(self.package_folder, "licenses"),
             src=self.source_folder)
        copy(self,
             "*.h",
             dst=os.path.join(self.package_folder, "include"),
             src=os.path.join(self.source_folder, "include"))
        copy(self,
             "*.hpp",
             dst=os.path.join(self.package_folder, "include"),
             src=os.path.join(self.source_folder, "include"))

        cmake = CMake(self)
        cmake.install()
=======
        bootstrap = self.python_requires["libhal-bootstrap"]
        bootstrap.module.add_library_requirements(self)
>>>>>>> f4d447ef5bfdd364fe1706a74a55a68e0ff85061

    def package_info(self):
        self.cpp_info.libs = ["libhal-mpu"]
        self.cpp_info.set_property("cmake_target_name", "libhal::mpu")
