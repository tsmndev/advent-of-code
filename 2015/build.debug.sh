conan install . --build=missing --settings=build_type=Debug
cd build/Debug/
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build .
