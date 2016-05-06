cmake_minimum_required (VERSION 2.6)
project (Scene_Example CXX)

set(PROJECT_LABEL "Scene")

link_directories (${LINK_DIRECTORY})
link_libraries (${LIBS})
SET ( SOURCE "./source/Scene.cpp")

set (COMMON_OUTPUT_DIR "./bin")
set (DEBUG_OUTPUT_DIR "./bin/Debug/")
set (RELEASE_OUTPUT_DIR "./bin/Release/")

set (PORTFOLIO_INCLUDE_DIR "../../include/")
set (TINYWINDOW_INCLUDE_DIR "../../dependencies/tinywindow/Include")
set (TINYEXTENDER_INCLUDE_DIR "../../dependencies/tinyextender/Include")
set (TINYSHADERS_INCLUDE_DIR "../../dependencies/tinyshaders/Include")
set (TINYCLOCK_INCLUDE_DIR "../../dependencies/tinyclock/Include")
set (GLM_INCLUDE_DIR "../../dependencies/glm/glm/")
set (FREEIMAGE_INCLUDE_DIR "../../dependencies/FreeImage/Dist/x32")
set (ANTTWEAKBAR_INCLUDE_DIR "../../dependencies/AntTweakBar/include")


include_directories("${PORTFOLIO_INCLUDE_DIR}")
include_directories("${TINYWINDOW_INCLUDE_DIR}")
include_directories("${TINYEXTENDER_INCLUDE_DIR}")
include_directories("${TINYSHADERS_INCLUDE_DIR}")
include_directories("${TINYCLOCK_INCLUDE_DIR}")
include_directories("${GLM_INCLUDE_DIR}")
include_directories("${FREEIMAGE_INCLUDE_DIR}")
include_directories("${ANTTWEAKBAR_INCLUDE_DIR}")
add_executable(Scene ${SOURCE} ${HEADER_FILES})
set_property(TARGET Scene PROPERTY OUTPUT_NAME "Scene")
