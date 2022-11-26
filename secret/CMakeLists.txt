
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)


add_subdirectory(client)
add_subdirectory(invoice_clerk)
add_subdirectory(mobility_provider)
add_subdirectory(platform)
add_subdirectory(proto)
add_subdirectory(transporter)
