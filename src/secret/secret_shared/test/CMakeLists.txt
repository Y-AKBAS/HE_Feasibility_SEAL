
set(Exec_Name SealUtil_TEST)
add_executable(${Exec_Name} SealUtilTest.cpp)
target_link_libraries(${Exec_Name} PRIVATE secretShared ${SHARED_LIBS})

PLACE_LOG_CONFIG(${Exec_Name}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )

add_test(
        NAME SealUtilTests
        COMMAND ${Exec_Name}
)