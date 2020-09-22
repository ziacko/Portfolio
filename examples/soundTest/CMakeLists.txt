cmake_minimum_required (VERSION 2.6)
project (Scene_Sound CXX)

set(PROJECT_LABEL "SoundTest")

link_directories (${LINK_DIRECTORY})
link_libraries (${LIBS})
set ( SOURCE "./source/Sound.cpp")

#set (TINYEXTENDER_SOURCE_DIR "../../dependencies/tinyextender/Source")

find_package(OpenGL)

if(UNIX)
set (LINK_DIRECTORY "/usr/lib/")
set (LIBS "-lX11 ${OPENGL_LIBRARIES}")
set (RELEASE_FLAGS "-std=c++11 -DSO -w -fpermissive 2> errors.txt")
set (DEBUG_FLAGS "-std=c++11 -DSO -w -g -fpermissive -DDEBUG 2> errors.txt")

if (CMAKE_BUILD_TYPE MATCHES Release)
	add_definitions(${RELEASE_FLAGS})
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(${DEBUG_FLAGS})
elseif (CMAKE_BUILD_TYPE MATCHES "")
	add_definitions(${DEBUG_FLAGS})
endif()

endif(UNIX)

set (COMMON_OUTPUT_DIR "./bin")
set (DEBUG_OUTPUT_DIR "./bin/Debug/")
set (RELEASE_OUTPUT_DIR "./bin/Release/")

set (PORTFOLIO_INCLUDE_DIR "../../include/")
set (TINYWINDOW_INCLUDE_DIR "../../dependencies/tinywindow/Include")
set (TINYWINDOW_DEPENDENCIES_DIR "../../dependencies/tinywindow/Dependencies")
set (TINYEXTENDER_INCLUDE_DIR "../../dependencies/tinyextender/Include")
set (TINYSHADERS_INCLUDE_DIR "../../dependencies/tinyshaders/Include")
set (TINYCLOCK_INCLUDE_DIR "../../dependencies/tinyclock/Include")
set (GLM_INCLUDE_DIR "../../dependencies/glm/")
#set (GLI_INCLUDE_DIR "../../dependencies/gli/")
set (IMGUI_INCLUDE_DIR "../../dependencies/imgui/")
set (STB_INCLUDE_DIR "../../dependencies/stb")
set (SOLOUD_INCLUDE_DIR "../../dependencies/Soloud/include")
set (SCENE_INCLUDE_DIR "./include")

set (COMMON_HEADER_DIR "../../include/")
set (COMMON_HEADERS 
"${COMMON_HEADER_DIR}Camera.h" 
"${COMMON_HEADER_DIR}DefaultUniformBuffer.h" 
"${COMMON_HEADER_DIR}Scene.h" 
"${COMMON_HEADER_DIR}Texture.h"
"${COMMON_HEADER_DIR}TexturedScene.h"
"${COMMON_HEADER_DIR}UniformBuffer.h"
"${COMMON_HEADER_DIR}Utilities.h"
"${COMMON_HEADER_DIR}VertexBuffer.h")

set (HEADERS "${COMMON_HEADERS}" "./include/SoundScene.h")

include_directories("${PORTFOLIO_INCLUDE_DIR}")
include_directories("${TINYWINDOW_INCLUDE_DIR}")
include_directories("${TINYWINDOW_DEPENDENCIES_DIR}")
include_directories("${TINYEXTENDER_INCLUDE_DIR}")
include_directories("${TINYSHADERS_INCLUDE_DIR}")
include_directories("${TINYCLOCK_INCLUDE_DIR}")
include_directories("${IMGUI_INCLUDE_DIR}")
include_directories("${GLM_INCLUDE_DIR}")
include_directories("${STB_INCLUDE_DIR}")
include_directories("${SOLOUD_INCLUDE_DIR}")
include_directories("${SCENE_INCLUDE_DIR}")
#include_directories("${GLI_INCLUDE_DIR}")

set(SOLOUD_LIB_DIRECTORY "../../dependencies/soloud/lib")

link_directories (${LINK_DIRECTORY})
link_libraries (${LIBS})

set (IMGUI_SOURCES "${IMGUI_INCLUDE_DIR}/imgui.cpp" "${IMGUI_INCLUDE_DIR}/imgui_draw" "${IMGUI_INCLUDE_DIR}/imgui_demo" "${IMGUI_INCLUDE_DIR}/imgui_widgets")

add_executable(SoundTest ${SOURCE} ${IMGUI_SOURCES} ${SOIL_SOURCES} ${HEADER_FILES} ${HEADERS})
set_property(TARGET SoundTest PROPERTY OUTPUT_NAME "SoundTest")

target_link_libraries(SoundTest "${SOLOUD_LIB_DIRECTORY}/soloud_static")
