
include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)

aux_source_directory(. SOURCE_FILES)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_directories(${PROJECT_NAME} PRIVATE ${Boost_LIBRARY_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE src/public SEAL::seal Boost::boost gRPC::grpc++ gRPC::grpc++_reflection
        gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf
        protobuf::libprotobuf-lite protobuf::libprotoc doctest::doctest log4cplus::log4cplus)