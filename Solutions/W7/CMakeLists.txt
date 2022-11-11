cmake_minimum_required(VERSION 3.14)

project(colors)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw3)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw3/include)

# GLee
set(GLee_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/GLee)
set(GLee_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/thirdparty/GLee/GLee.c)
include_directories(${GLee_INCLUDE_DIRS})

add_definitions(-DMAIN_DIR="${CMAKE_CURRENT_LIST_DIR}/")	
add_executable(colors colors.cpp ${GLee_LIBRARIES})

target_link_libraries(colors ${OPENGL_LIBRARIES})
target_link_libraries(colors glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})
message("finished configuration")
