

set(Exec_Name SharedDir_TEST)
add_executable(${Exec_Name} SharedDirTest.cpp)
target_link_libraries(${Exec_Name} PRIVATE ${SHARED_LIBS})
target_include_directories(${Exec_Name} PUBLIC ${Boost_INCLUDE_DIR})

PLACE_LOG_CONFIG(${Exec_Name}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )

add_test(
        NAME SharedDirTests
        COMMAND ${Exec_Name}
)
