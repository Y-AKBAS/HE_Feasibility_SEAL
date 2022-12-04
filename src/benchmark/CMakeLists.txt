
aux_source_directory(. SOURCE_FILES)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#target_link_directories(${PROJECT_NAME} PRIVATE ${Boost_LIBRARY_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE SEAL::seal)