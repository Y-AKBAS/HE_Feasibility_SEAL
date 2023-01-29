
aux_source_directory(. SOURCE_FILES)

set(TARGET_NAME publicClient)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE ${SHARED_LIBS} publicShared)

add_library(publicClientLibrary ${SOURCE_FILES})
target_link_libraries(publicClientLibrary PRIVATE ${SHARED_LIBS} PUBLIC publicShared)
target_include_directories(publicClientLibrary PUBLIC .)

PLACE_LOG_CONFIG(${TARGET_NAME}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )

CREATE_APP_TEST(${TARGET_NAME})
