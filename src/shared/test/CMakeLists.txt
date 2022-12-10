

set(Exec_Name SharedDir_TEST)
add_executable(${Exec_Name} SharedDirTest.cpp)
target_link_libraries(${Exec_Name} ${SHARED_LIBS})

PLACE_LOG_CONFIG(${Exec_Name}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )

add_test(
        NAME SharedDirTests
        COMMAND ${Exec_Name}
)
