project(socket_net)

add_library("${PROJECT_NAME}" STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/csocket_exception.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ithread.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/csocket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/imain_socket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ctcp_client.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cudp_client.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cudp_mcast_client.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ctcp_server.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cudp_server.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cudp_mcast_server.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cgenerators.cc"
)
target_link_libraries("${PROJECT_NAME}" PRIVATE ${CMAKE_THREAD_LIBS_INIT})
if (MSVC)
    target_link_libraries("${PROJECT_NAME}" PRIVATE wsock32 ws2_32)
endif()
target_link_libraries("${PROJECT_NAME}" PRIVATE Threads::Threads)
target_compile_definitions("${PROJECT_NAME}" PRIVATE -DNET_REUSEADDR)

add_library(ExampleHelper STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/cstring_package.cc"
)

add_executable(TcpClient "${CMAKE_CURRENT_SOURCE_DIR}/tcp_client_main.cc")
target_link_libraries(TcpClient PUBLIC 
    "${PROJECT_NAME}"
    ExampleHelper
)

add_executable(TcpServer "${CMAKE_CURRENT_SOURCE_DIR}/tcp_server_main.cc")
target_link_libraries(TcpServer PUBLIC 
    "${PROJECT_NAME}"
    ExampleHelper
)

add_executable(UdpClient "${CMAKE_CURRENT_SOURCE_DIR}/udp_client_main.cc")
target_link_libraries(UdpClient PUBLIC 
    "${PROJECT_NAME}"
    ExampleHelper
)

add_executable(UdpServer "${CMAKE_CURRENT_SOURCE_DIR}/udp_server_main.cc")
target_link_libraries(UdpServer PUBLIC 
    "${PROJECT_NAME}"
    ExampleHelper
)

add_executable(UdpMulticastClient "${CMAKE_CURRENT_SOURCE_DIR}/udp_mcast_client_main.cc")
target_link_libraries(UdpMulticastClient PUBLIC 
    "${PROJECT_NAME}"
    ExampleHelper
)

add_executable(UdpMulticastServer "${CMAKE_CURRENT_SOURCE_DIR}/udp_mcast_server_main.cc")
target_link_libraries(UdpMulticastServer PUBLIC 
    "${PROJECT_NAME}"
    ExampleHelper
)
