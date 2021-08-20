project(socket_net)

add_library(${PROJECT_NAME} STATIC
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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/socket_utils.cc"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DNET_REUSEADDR)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
if (MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE wsock32 ws2_32)
endif()
