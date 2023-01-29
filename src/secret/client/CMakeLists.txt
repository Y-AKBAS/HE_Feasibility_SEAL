
aux_source_directory(. SOURCE_FILES)

set(TARGET_NAME secretClient)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE ${SHARED_LIBS} secretShared)

add_library(secretClientLibrary ${SOURCE_FILES})
target_link_libraries(secretClientLibrary PRIVATE ${SHARED_LIBS} PUBLIC secretShared)
target_include_directories(secretClientLibrary PUBLIC .)

PLACE_LOG_CONFIG(${TARGET_NAME}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )

CREATE_APP_TEST(${TARGET_NAME})
