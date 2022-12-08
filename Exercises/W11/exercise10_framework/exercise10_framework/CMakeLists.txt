cmake_minimum_required(VERSION 3.14)

project(LightingAndShading)


file(GLOB SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Helpers/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Helpers/*.cpp
)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${OPENGL_INCLUDE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# GDIPLUS for Windows
if(MSVC)
	find_package(GDIPLUS REQUIRED)
endif(MSVC)

# libpng for Unix
if (UNIX)
	find_package(PNG REQUIRED)
	include_directories(${PNG_INCLUDE_DIR})
endif(UNIX)
					
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
	
add_executable(LightingAndShading ${SOURCES} ${GLee_LIBRARIES})

if(MSVC)
	target_link_libraries(LightingAndShading ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GDIPLUS_LIBRARY})
else (MSVC)
	target_link_libraries(LightingAndShading ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${PNG_LIBRARY})
endif(MSVC)
