
add_compile_definitions(DEFAULT_LOG_CONFIG_FILE_NAME="DefaultLogConfiguration.ini")

function(PLACE_LOG_CONFIG TARGET_EXE)

    set(OPTIONS_PARAMS "")
    set(SINGLE_VALUE_PARAMS CONFIG_FILE_DIR)
    set(MULTI_VALUE_PARAMS "")

    cmake_parse_arguments(ARG
            "${OPTIONS_PARAMS}"
            "${SINGLE_VALUE_PARAMS}"
            "${MULTI_VALUE_PARAMS}"
            "${ARGN}")

    if (NOT ARG_CONFIG_FILE_DIR)
        message(FATAL_ERROR "Please provide both log config file dir")
    endif ()

    get_filename_component(FILE_NAME "${ARG_CONFIG_FILE_DIR}" NAME)

    add_custom_command(TARGET ${TARGET_EXE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${ARG_CONFIG_FILE_DIR}
            $<TARGET_FILE_DIR:${TARGET_EXE}>)

endfunction()