
    set(CPP_PLUGIN_PATH "C:/vcpkg/packages/grpc_x64-windows/tools/grpc/grpc_cpp_plugin.exe")
    set(PROTOC_PATH "C:/vcpkg/packages/protobuf_x64-windows/tools/protobuf/protoc.exe")

    function(GENERATE_PROTO_FILES RESULT_FILES)

        set(OPTIONS_PARAMS "")
        set(SINGLE_VALUE_PARAMS PROTO_FILE_ABSOLUTE OUTPUT_DIR)
        set(MULTI_VALUE_PARAMS "")

        cmake_parse_arguments(ARG
                "${OPTIONS_PARAMS}"
                "${SINGLE_VALUE_PARAMS}"
                "${MULTI_VALUE_PARAMS}"
                "${ARGN}")

        # check against missing args
        if (NOT ARG_PROTO_FILE_ABSOLUTE)
            message(FATAL_ERROR "PROTO_FILE is missing")
        elseif (NOT ARG_OUTPUT_DIR)
            message(FATAL_ERROR "OUTPUT_DIR is missing")
        endif ()

        get_filename_component(proto_file_dir "${ARG_PROTO_FILE_ABSOLUTE}" PATH)
        get_filename_component(proto_file_name "${ARG_PROTO_FILE_ABSOLUTE}" NAME_WE)

        set(proto_src ${ARG_OUTPUT_DIR}/${proto_file_name}.pb.cc)
        set(proto_header ${ARG_OUTPUT_DIR}/${proto_file_name}.pb.h)
        set(grpc_src ${ARG_OUTPUT_DIR}/${proto_file_name}.grpc.pb.cc)
        set(grpc_header ${ARG_OUTPUT_DIR}/${proto_file_name}.grpc.pb.h)

        # There are other options to create proto files too with add_custom_command()
        # or add_custom_target(). But I wanted to regenerate them everytime I rerun cmake
        # to keep them upto date.
        execute_process(COMMAND ${PROTOC_PATH}
                -I=${proto_file_dir} --grpc_out=${ARG_OUTPUT_DIR}
                --plugin=protoc-gen-grpc=${CPP_PLUGIN_PATH} ${ARG_PROTO_FILE_ABSOLUTE})

        execute_process(COMMAND ${PROTOC_PATH}
                -I=${proto_file_dir} --cpp_out=${ARG_OUTPUT_DIR}
                ${ARG_PROTO_FILE_ABSOLUTE})

        set(${RESULT_FILES} "${proto_src}" "${grpc_src}" PARENT_SCOPE)

        #add_library("${LIB_NAME}" STATIC "${proto_src}" "${grpc_src}")
        #target_include_directories("${LIB_NAME}" PUBLIC ${ARG_OUTPUT_DIR})

    endfunction()
