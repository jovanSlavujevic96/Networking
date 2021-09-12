cmake_minimum_required(VERSION 3.19)
project(Networking)

# Disable in-source builds to prevent source tree corruption.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message( FATAL_ERROR 
        "FATAL: In-source builds are not allowed. "
        "You should create a separate directory for build files."
    )
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

if (${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_CURRENT_SOURCE_DIR})
    if (MSVC)
        # warning level 4 and all warnings as errors
        add_compile_options(/W4 /WX)
    else()
        # lots of warnings and all warnings as errors
        add_compile_options(-Wall -Wextra -pedantic -Werror)
    endif()
endif (${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_CURRENT_SOURCE_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/socket_net/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/socket_net")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/apps")

add_library(ExampleHelper STATIC "${CMAKE_CURRENT_SOURCE_DIR}/apps/cstring_package.cc")

link_libraries(socket_net)
link_libraries(ExampleHelper)

add_executable(TcpClient "${CMAKE_CURRENT_SOURCE_DIR}/apps/tcp_client_main.cc")
add_executable(TcpServer "${CMAKE_CURRENT_SOURCE_DIR}/apps/tcp_server_main.cc")
add_executable(UdpUnicastClient "${CMAKE_CURRENT_SOURCE_DIR}/apps/udp_client_main.cc")
add_executable(UdpUnicastServer "${CMAKE_CURRENT_SOURCE_DIR}/apps/udp_server_main.cc")
add_executable(UdpMulticastClient "${CMAKE_CURRENT_SOURCE_DIR}/apps/udp_mcast_client_main.cc")
add_executable(UdpMulticastServer "${CMAKE_CURRENT_SOURCE_DIR}/apps/udp_mcast_server_main.cc")
