cmake_minimum_required(VERSION 3.10)

project(shaders)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLUT
find_package(GLUT)
if(NOT GLUT_FOUND)
	include(${CMAKE_CURRENT_LIST_DIR}/thirdparty/freeglut/CMakeLists.txt)

	set(GLUT_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/freeglut/include)
	set(GLUT_LIBRARIES freeglut_static)
endif(NOT GLUT_FOUND)
include_directories(${GLUT_INCLUDE_DIRS})

# GLee
set(GLee_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/GLee)
set(GLee_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/thirdparty/GLee/GLee.c)
include_directories(${GLee_INCLUDE_DIRS})

add_definitions(-DMAIN_DIR="${CMAKE_CURRENT_LIST_DIR}/")

file(GLOB files
		MainProgram.h
		MainProgram.cpp
		Shaders.cpp
		Helpers/GLShader.h
		Helpers/GLShader.cpp
		Helpers/opengl.h
	)
	
add_executable(shaders ${files} ${GLee_LIBRARIES})

target_link_libraries(shaders ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
