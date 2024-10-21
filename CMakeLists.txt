cmake_minimum_required(VERSION 3.30)
# Include vcpkg integration
set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg target triplet")

# Set the project name and C++ standard
project(CppKeep LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find the CURL library
find_package(CURL REQUIRED)

# Add the executable
add_executable(keep_client src/keep_client.cpp src/main.cpp)

target_include_directories(keep_client PUBLIC include)

# Link the CURL library
target_link_libraries(keep_client CURL::libcurl)
