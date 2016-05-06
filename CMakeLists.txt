cmake_minimum_required(VERSION 2.6)
project (Portfolio CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (GNUInstallDirs)
include (CMakePackageConfigHelpers)

set (PROJECT_LABEL "Portfolio")
add_definitions (-D_CRT_SECURE_NO_WARNINGS)

find_Package(OpenGL)

if(UNIX)
	set (LINK_DIRECTORY "/usr/lib")
	set (LIBS "-X11 ${OPENGL_LIBRARIES}")
	set (COMMON_FLAGS "-std=c++11 -DSO -Wall -Wextra")
	set (DEBUG_FLAGS "-g -DDEBUG")
	set (ERROR_OUTPUT "2> errors.txt")
	
	if(CMAKE_BUILD_TYPE MATCHES Release)
		add_definitions(${COMMON_FLAGS} ${ERROR_OUTPUT})
	elseif (CMAKE_BUILD_TYPE MATCHES Debug)
		add_definitions(${COMMON_FLAGS} ${DEBUG_FLAGS} ${ERROR_OUTPUT})
	elseif(CMAKE_BUILD_TYPE MATCHES "")
		add_definitions(${COMMON_FLAGS} ${DEBUG_FLAGS} ${ERROR_OUTPUT})
	endif()
endif(UNIX)

if(WIN32)
	set (LIBS "${OPENGL_LIBRARIES}")
endif(WIN32)

set (COMMON_OUTPUT_DIR "./bin")
set (DEBUG_OUTPUT_DIR "./bin/Debug/")
set (RELEASE_OUTPUT_DIR "./bin/Release/")

#set (PORTFOLIO_INCLUDE_DIR "./include")
#set (TINYWINDOW_INCLUDE_DIR "./dependencies/tinywindow/Include")
#set (TINYEXTENDER_INCLUDE_DIR "./dependencies/tinyextender/Include")
#set (TINYSHADERS_INCLUDE_DIR "./dependencies/tinyextender/Include")
#set (TINYCLOCK_INCLUDE_DIR "./dependencies/tinyclock/Include")
#set (GLM_INCLUDE_DIR "./dependencies/glm/glm/")
#set (FREEIMAGE__INCLUDE_DIR "./dependencies/FreeImage/Dist/x32")
#set (COMMON_INCLUDE_DIRS "${PORTFOLIO_INCLUDE_DIR} ${TINYWINDOW_INCLUDE_DIR} ${TINYEXTENDER_INCLUDE_DIR} ${TINYSHADERS_INCLUDE_DIR} ${TINYCLOCK_INCLUDE_DIR} ${GLM_INCLUDE_DIR}")

#include_directories("${PORTFOLIO_INCLUDE_DIR}")
#include_directories("${TINYWINDOW_INCLUDE_DIR}")
#include_directories("${TINYEXTENDER_INCLUDE_DIR}")
#include_directories("${TINYSHADERS_INCLUDE_DIR}")
#include_directories("${TINYCLOCK_INCLUDE_DIR}")
#include_directories("${GLM_INCLUDE_DIR}")

set (TINYWINDOW_EXAMPLE_DIR "./dependencies/tinywindow/Example")
set (TINYEXTENDER_EXAMPLE_DIR "./dependencies/tinyextender/Example")
set (TINYSHADERS_EXAMPLE_DIR "./dependencies/tinyshaders/Example")
set (TINYCLOCK_EXAMPLE_DIR "./dependencies/tinyclock/Example")

#build dependencies
add_subdirectory("${TINYWINDOW_EXAMPLE_DIR}")
add_subdirectory("${TINYEXTENDER_EXAMPLE_DIR}")
add_subdirectory("${TINYSHADERS_EXAMPLE_DIR}")
add_subdirectory("${TINYCLOCK_EXAMPLE_DIR}")

#build examples
add_subdirectory("./Examples/Scene/")
