cmake_minimum_required(VERSION 2.8.11)
project(raycaster)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra")
endif()

add_executable(raycaster main.c)

# Add GLFW
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
target_link_libraries(raycaster glfw ${GLFW_LIBRARIES})
