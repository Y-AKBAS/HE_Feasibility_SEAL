cmake_minimum_required(VERSION 3.23)
project(HE_Feasibility_SEAL
        LANGUAGES CXX
        VERSION 1.0.0)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

get_filename_component(VCPKG_CMAKE_DIR "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" ABSOLUTE)
include(${VCPKG_CMAKE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

enable_testing()
add_subdirectory(src)
