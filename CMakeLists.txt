cmake_minimum_required(VERSION 3.23)
project(HE_Feasibility_SEAL
        LANGUAGES CXX
        VERSION 1.0.0)

set(CMAKE_CXX_STANDARD_REQUIRED 23)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PackageFinder)


add_subdirectory(src)
