CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

SET(FREEGLUT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/GL/freeglut.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GL/freeglut_ext.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GL/freeglut_std.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GL/glut.h
)

SET(FREEGLUT_SRCS
    ${FREEGLUT_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_cursor.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_display.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_ext.c
	${CMAKE_CURRENT_LIST_DIR}/src/freeglut_font.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_font_data.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_gamemode.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_geometry.c
	${CMAKE_CURRENT_LIST_DIR}/src/freeglut_glutfont_definitions.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_init.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_input_devices.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_joystick.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_main.c
	${CMAKE_CURRENT_LIST_DIR}/src/freeglut_menu.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_misc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_overlay.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_spaceball.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_state.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_stroke_mono_roman.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_stroke_roman.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_structure.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_teapot.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_teapot_data.h
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_videoresize.c
    ${CMAKE_CURRENT_LIST_DIR}/src/freeglut_window.c
	${CMAKE_CURRENT_LIST_DIR}/src/freeglut_xinput.c
)

add_definitions(-DFREEGLUT_STATIC)

IF(WIN32)
	add_definitions(-DTARGET_HOST_MS_WINDOWS)
ELSEIF(APPLE)
	add_definitions(-DTARGET_HOST_MAC_OSX)
ELSEIF(UNIX)
	add_definitions(-DTARGET_HOST_POSIX_X11)
	add_definitions(-DHAVE_FCNTL_H)
ENDIF()

add_library(freeglut_static STATIC ${FREEGLUT_SRCS})

IF (NOT WIN32)
	target_link_libraries(freeglut_static m rt X11)
ENDIF(NOT WIN32)
