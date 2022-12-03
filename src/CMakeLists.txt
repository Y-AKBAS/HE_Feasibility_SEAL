
SET(SHARED_LIBS "")

include(PackageFinder)
include(ApplicationConfig)
include(LogConfigurer)

add_subdirectory(proto)
add_subdirectory(shared)
add_subdirectory(public)
add_subdirectory(secret)
add_subdirectory(benchmark)