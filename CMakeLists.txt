cmake_minimum_required(VERSION 3.23)
project(HE_Feasibility_SEAL)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)

add_executable(HE_Feasibility_SEAL Main.cpp Application.cpp Application.h)

find_package(SEAL 4.0.0 REQUIRED)
find_package(Boost 1.80.0 REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(log4cplus CONFIG REQUIRED)

add_subdirectory(shared)
add_subdirectory(public)
add_subdirectory(secret)

target_link_directories(${PROJECT_NAME} PUBLIC ${Boost_LIBRARY_DIR} )

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC SEAL::seal Boost::boost gRPC::grpc++ gRPC::grpc++_reflection
        gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf
        protobuf::libprotobuf-lite protobuf::libprotoc doctest::doctest log4cplus::log4cplus)
