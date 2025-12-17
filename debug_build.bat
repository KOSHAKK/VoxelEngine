conan install . -s build_type=Debug --build=missing
cmake -S . -B build --preset conan-deb