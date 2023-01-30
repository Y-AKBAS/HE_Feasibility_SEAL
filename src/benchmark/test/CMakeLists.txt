
set(TARGET_NAME GoogleBenchmark)
add_executable(${TARGET_NAME} GoogleBenchmark.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC ${SHARED_LIBS} publicClientLibrary secretClientLibrary
        benchmark::benchmark benchmark::benchmark_main)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)

PLACE_LOG_CONFIG(${TARGET_NAME}
        CONFIG_FILE_DIR "${DEFAULT_LOG_CONFIGURER}"
        )