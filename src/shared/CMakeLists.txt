
aux_source_directory(. SOURCE_FILES)

add_library(shared "${SOURCE_FILES}")
target_link_libraries(shared PUBLIC gRPC::grpc++)
target_include_directories(shared PUBLIC .)