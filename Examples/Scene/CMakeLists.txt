cmake_minimum_required (VERSION 2.6)
project (Scene_Example CXX)

set(PROJECT_LABEL "Scene")

link_directories (${LINK_DIRECTORY})
link_libraries (${LIBS})
set ( SOURCE "./source/Scene.cpp")

#set (TINYEXTENDER_SOURCE_DIR "../../dependencies/tinyextender/Source")

set (COMMON_OUTPUT_DIR "./bin")
set (DEBUG_OUTPUT_DIR "./bin/Debug/")
set (RELEASE_OUTPUT_DIR "./bin/Release/")

set (PORTFOLIO_INCLUDE_DIR "../../include/")
set (TINYWINDOW_INCLUDE_DIR "../../dependencies/tinywindow/Include")
set (TINYEXTENDER_INCLUDE_DIR "../../dependencies/tinyextender/Include")
set (TINYSHADERS_INCLUDE_DIR "../../dependencies/tinyshaders/Include")
set (TINYCLOCK_INCLUDE_DIR "../../dependencies/tinyclock/Include")
set (GLM_INCLUDE_DIR "../../dependencies/glm/glm/")
set (IMGUI_INCLUDE_DIR "../../dependencies/imGUI/")
set (SOIL_INCLUDE_DIR "../../dependencies/SOIL/src")

set (COMMON_HEADER_DIR "../../include/")
set (COMMON_HEADERS 
"${COMMON_HEADER_DIR}Camera.h" 
"${COMMON_HEADER_DIR}/DefaultUniformBuffer.h" 
"${COMMON_HEADER_DIR}Scene.h" 
"${COMMON_HEADER_DIR}Texture.h"
"${COMMON_HEADER_DIR}TexturedScene.h"
"${COMMON_HEADER_DIR}UniformBuffer.h"
"${COMMON_HEADER_DIR}Utilities.h"
"${COMMON_HEADER_DIR}VertexBuffer.h")

set (HEADERS "${COMMON_HEADERS}")


include_directories("${PORTFOLIO_INCLUDE_DIR}")
include_directories("${TINYWINDOW_INCLUDE_DIR}")
include_directories("${TINYEXTENDER_INCLUDE_DIR}")
include_directories("${TINYSHADERS_INCLUDE_DIR}")
include_directories("${TINYCLOCK_INCLUDE_DIR}")
include_directories("${IMGUI_INCLUDE_DIR}")
include_directories("${GLM_INCLUDE_DIR}")
include_directories("${SOIL_INCLUDE_DIR}")

set (IMGUI_SOURCES "${IMGUI_INCLUDE_DIR}/imgui.cpp" "${IMGUI_INCLUDE_DIR}/imgui_draw" "${IMGUI_INCLUDE_DIR}/imgui_demo")

add_executable(Scene ${SOURCE} ${IMGUI_SOURCES} ${HEADER_FILES} ${HEADERS})
set_property(TARGET Scene PROPERTY OUTPUT_NAME "Scene")
