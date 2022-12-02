
# define port numbers for applications
set(SECRET_PREFIX SECRET_)
set(PUBLIC_PREFIX PUBLIC_)
set(LOCALHOST "127.0.0.1:")

add_compile_definitions(${SECRET_PREFIX}INVOICE_CLERK_SERVER_PORT="${LOCALHOST}8081")
add_compile_definitions(${SECRET_PREFIX}MOBILITY_PROVIDER_SERVER_PORT="${LOCALHOST}8082")
add_compile_definitions(${SECRET_PREFIX}PLATFORM_SERVER_PORT="${LOCALHOST}8083")
add_compile_definitions(${SECRET_PREFIX}TRANSPORTER_SERVER_PORT="${LOCALHOST}8084")

add_compile_definitions(${PUBLIC_PREFIX}INVOICE_CLERK_SERVER_PORT="${LOCALHOST}8085")
add_compile_definitions(${PUBLIC_PREFIX}MOBILITY_PROVIDER_SERVER_PORT="${LOCALHOST}8086")
add_compile_definitions(${PUBLIC_PREFIX}PLATFORM_SERVER_PORT="${LOCALHOST}8087")
add_compile_definitions(${PUBLIC_PREFIX}TRANSPORTER_SERVER_PORT="${LOCALHOST}8088")