
include(ProtoHandler)

# create secret_shared proto
get_filename_component(PROTO_FILE_ABSOLUTE_PATH "SharedCommunication.proto" ABSOLUTE)

GENERATE_PROTO_FILES(SHARED_PROTO_FILES
        PROTO_FILE_ABSOLUTE "${PROTO_FILE_ABSOLUTE_PATH}"
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(SHARED_PROTO_FILES "${SHARED_PROTO_FILES}" PARENT_SCOPE)

# create secret proto
get_filename_component(PROTO_FILE_ABSOLUTE_PATH "SecretCommunication.proto" ABSOLUTE)

GENERATE_PROTO_FILES(SECRET_PROTO_FILES
        PROTO_FILE_ABSOLUTE "${PROTO_FILE_ABSOLUTE_PATH}"
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND SECRET_PROTO_FILES "${SHARED_PROTO_FILES}")
set(SECRET_PROTO_FILES "${SECRET_PROTO_FILES}" PARENT_SCOPE)

# create public proto
get_filename_component(PROTO_FILE_ABSOLUTE_PATH "PublicCommunication.proto" ABSOLUTE)

GENERATE_PROTO_FILES(PUBLIC_PROTO_FILES
        PROTO_FILE_ABSOLUTE "${PROTO_FILE_ABSOLUTE_PATH}"
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND PUBLIC_PROTO_FILES "${SHARED_PROTO_FILES}")
set(PUBLIC_PROTO_FILES "${PUBLIC_PROTO_FILES}" PARENT_SCOPE)

# create libs
set(GRPC_LIBS gRPC::grpc++ gRPC::grpc++_reflection protobuf::libprotoc protobuf::libprotobuf)

add_library(public_proto STATIC "${SHARED_PROTO_FILES}" "${PUBLIC_PROTO_FILES}")
target_link_libraries(public_proto PRIVATE "${GRPC_LIBS}")
target_include_directories(public_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_library(secret_proto STATIC "${SHARED_PROTO_FILES}" "${SECRET_PROTO_FILES}")
target_link_libraries(secret_proto PRIVATE "${GRPC_LIBS}")
target_include_directories(secret_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND SHARED_LIBS "${GRPC_LIBS}" secret_proto public_proto)
set(SHARED_LIBS "${SHARED_LIBS}" PARENT_SCOPE)
