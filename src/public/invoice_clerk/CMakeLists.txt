
aux_source_directory(. SOURCE_FILES)

set(TARGET_NAME publicInvoiceClerk)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE ${SHARED_LIBS})

PLACE_LOG_CONFIG(${TARGET_NAME}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
)

CREATE_APP_TEST(${TARGET_NAME})
