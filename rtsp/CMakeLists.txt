project(Rtsp)

add_library("${PROJECT_NAME}" STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/src/crtsp_master.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/crtsp_client_handler.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/crtsp_media_session.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/crtsp_request_package.cc"
)
target_compile_definitions("${PROJECT_NAME}" PUBLIC -D_DEBUG)
target_link_libraries("${PROJECT_NAME}" PUBLIC 
	socket_net
	Threads::Threads
)

add_executable("${PROJECT_NAME}_app" "${CMAKE_CURRENT_SOURCE_DIR}/rtsp_main.cc")
target_link_libraries("${PROJECT_NAME}_app" PUBLIC "${PROJECT_NAME}")
