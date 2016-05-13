# Copyright (C) 2016 Luiz Carlos Vieira (http://www.luiz.vieira.nom.br)
#
# This file is part of GameCap.
#
# GameCap is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GameCap is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Minimum CMake version required 
cmake_minimum_required(VERSION 2.8.11)

# Project information
project (GameCap)

# Version data and header file
set (GC_VERSION_MAJOR 1)
set (GC_VERSION_MINOR 0)
set (GC_VERSION_PATCH 0)
set (GC_VERSION "${GC_VERSION_MAJOR}.${GC_VERSION_MINOR}.${GC_VERSION_PATCH}")
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

# Qt configuration
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTORCC ON) # Instruct CMake to run rcc automatically when needed
set(CMAKE_AUTOUIC ON) # Instruct CMake to run uic automatically when needed
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

# Add source files
file(GLOB SRC_FILES src/*.cpp src/*.h)

# Add translation files (via a dynamic resource file)
file(GLOB TS_FILES src/translations/*.ts)
set(QM_LIST "")
foreach(TS_FILE ${TS_FILES})
    get_filename_component (TS_FILE_WE ${TS_FILE} NAME_WE)
    set(QM_LIST "${QM_LIST}<file>${TS_FILE_WE}.qm</file>\n")
endforeach()
configure_file (
  "${PROJECT_SOURCE_DIR}/translations.qrc.in"
  "${PROJECT_BINARY_DIR}/translations.qrc"
)

# Update the TS files
option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts")
if (UPDATE_TRANSLATIONS)
  QT5_CREATE_TRANSLATION(QM_FILES ${SRC_FILES} ${TS_FILES})
else (UPDATE_TRANSLATIONS)
  QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES})
endif (UPDATE_TRANSLATIONS)

# prevent the generated files from being deleted during make clean
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)

# Add resource files
file(GLOB RSC_FILES src/*.qrc ${PROJECT_BINARY_DIR}/*.qrc)

# Add the executable
if(WIN32)
    add_executable(GameCap WIN32 ${SRC_FILES} ${RSC_FILES})
else()
    add_executable(GameCap ${SRC_FILES} ${RSC_FILES})
endif()
set_target_properties(GameCap PROPERTIES OUTPUT_NAME gamecap)
set_target_properties(GameCap PROPERTIES OUTPUT_NAME_DEBUG gamecapd)

# Set up the required libraries
target_link_libraries(GameCap Qt5::Widgets)