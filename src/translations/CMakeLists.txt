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

# Add the translation files (via a dynamic resource file)
find_package(Qt5LinguistTools REQUIRED)

# GLOB is not used to set TS_FILES because they might not exist
# yet (i.e. when they are created for the first time from source files)
set(TS_FILES ${PROJECT_SOURCE_DIR}/src/translations/pt-br.ts)
set(NEW_LINE "")
foreach(TS_FILE ${TS_FILES})
    get_filename_component(TS_FILE_WE ${TS_FILE} NAME_WE)
    set(QM_LIST "${QM_LIST}${NEW_LINE}\t\t<file>${TS_FILE_WE}.qm</file>")
    if(NOT NEW_LINE)
        set(NEW_LINE "\n")
    endif()
endforeach()
configure_file(
  "${PROJECT_SOURCE_DIR}/src/translations/translations.qrc.in"
  "${PROJECT_BINARY_DIR}/src/translations/translations.qrc"
)

# Option to allow turning on the creation of the TS files
option (CREATE_TRANSLATIONS "Create/update the translation files (*.ts) if true, otherwise just add the existing translations to the project. WARNING! If this option is on, then the 'make clean' in the Translations target will remove the translation files!")

# Create or add the TS files
if(CREATE_TRANSLATIONS)
    qt5_create_translation(QM_FILES ${SRC_FILES} ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

# Add the custom target for the translations
add_custom_target (Translations DEPENDS ${QM_FILES})