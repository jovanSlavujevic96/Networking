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

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/socket_net/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/rtsp/include")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/socket_net")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/rtsp")
