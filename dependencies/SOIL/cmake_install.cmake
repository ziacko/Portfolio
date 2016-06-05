# Install script for directory: /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/libSOIL.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SOIL" TYPE FILE FILES
    "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.h"
    "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.h"
    "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.h"
    "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.h"
    "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stbi_DDS_aug_c.h"
    "/home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stbi_DDS_aug.h"
    )
endif()

