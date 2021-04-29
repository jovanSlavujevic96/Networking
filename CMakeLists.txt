cmake_minimum_required(VERSION 3.19)
project(net)

# Disable in-source builds to prevent source tree corruption.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message( FATAL_ERROR 
        "FATAL: In-source builds are not allowed. "
        "You should create a separate directory for build files."
    )
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library("${PROJECT_NAME}" STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/csocket_exception.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cstring_package.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ithread.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/csocket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/imain_socket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ctcp_client.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cudp_client.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ctcp_server.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cudp_server.cc"
)
target_link_libraries("${PROJECT_NAME}" PRIVATE ${CMAKE_THREAD_LIBS_INIT})
if (MSVC)
    target_link_libraries("${PROJECT_NAME}" PRIVATE wsock32 ws2_32)
endif()
target_compile_definitions("${PROJECT_NAME}" PRIVATE -DNET_REUSEADDR)


add_executable(TcpClient tcp_client_main.cc)
target_link_libraries(TcpClient PUBLIC "${PROJECT_NAME}")

add_executable(TcpServer tcp_server_main.cc)
target_link_libraries(TcpServer PUBLIC "${PROJECT_NAME}")

add_executable(UdpClient udp_client_main.cc)
target_link_libraries(UdpClient PUBLIC "${PROJECT_NAME}")

add_executable(UdpServer udp_server_main.cc)
target_link_libraries(UdpServer PUBLIC "${PROJECT_NAME}")
