
aux_source_directory(. SOURCE_FILES)

set(TARGET_NAME benchmark)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PUBLIC ${SHARED_LIBS} publicClientLibrary secretClientLibrary)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)

PLACE_LOG_CONFIG(${TARGET_NAME}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )