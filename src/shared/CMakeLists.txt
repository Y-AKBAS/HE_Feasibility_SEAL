
aux_source_directory(. SOURCE_FILES)

message("Sources: ${SOURCE_FILES}")

set(TEST_EXEC_FILE "./TestExecutable.cpp")
list(REMOVE_ITEM SOURCE_FILES ${TEST_EXEC_FILE})

add_library(sharedLib STATIC "${SOURCE_FILES}" IApplication.cpp)
target_link_libraries(sharedLib PUBLIC Boost::boost doctest::doctest log4cplus::log4cplus)
target_include_directories(sharedLib PUBLIC .)

list(APPEND SHARED_LIBS sharedLib)
set(SHARED_LIBS "${SHARED_LIBS}" PARENT_SCOPE)

get_filename_component(DEFAULT_LOG_CONFIGURER "DefaultLogConfiguration.ini" ABSOLUTE)
set(DEFAULT_LOG_CONFIGURER "${DEFAULT_LOG_CONFIGURER}" PARENT_SCOPE)

set(TEST_EXEC "TEST_EXEC")
add_executable(${TEST_EXEC} ${TEST_EXEC_FILE})
target_link_libraries(${TEST_EXEC} PRIVATE doctest::doctest)
set_property(GLOBAL PROPERTY TEST_EXEC ${TEST_EXEC})

add_subdirectory(test)
