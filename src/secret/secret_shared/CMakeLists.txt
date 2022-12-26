
aux_source_directory(. SOURCE_FILES)

set(TARGET_NAME secretShared)
add_library(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME}
        PRIVATE ${SHARED_LIBS}
        PUBLIC SEAL::seal
        )
target_include_directories(${TARGET_NAME} PUBLIC .)

PLACE_LOG_CONFIG(${TARGET_NAME}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )

add_subdirectory(test)