
aux_source_directory(. SOURCE_FILES)

# Add header only util file
list(APPEND SOURCE_FILES Utils.h)

add_library(sharedLib STATIC "${SOURCE_FILES}")
target_link_libraries(sharedLib PUBLIC Boost::boost doctest::doctest log4cplus::log4cplus)
target_include_directories(sharedLib PUBLIC .)

list(APPEND SHARED_LIBS sharedLib)
set(SHARED_LIBS "${SHARED_LIBS}" PARENT_SCOPE)

get_filename_component(DEFAULT_LOG_CONFIGURER "DefaultLogConfiguration.ini" ABSOLUTE)
set(DEFAULT_LOG_CONFIGURER "${DEFAULT_LOG_CONFIGURER}" PARENT_SCOPE)