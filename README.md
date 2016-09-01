# Gameplay Data Capture Tool

This is a tool used to capture the gameplay data used in creation of the PhD project named [Fun From Faces](http://www.luiz.vieira.nom.br/phd-research/). The data is mainly composed of video recordings from players' faces and the game screen, as well as self-reports on the levels of fun, frustration and involvement experienced.

## Dependencies

This project depends on the following software:

 * **[Open Broadcaster Software (OBS Studio)](https://obsproject.com/)**. OBS Studio is a free (open-source) tool for streaming and recording gameplay videos. It is used in this project for recording both the player's face and the game screen in separated but syncronized videos. The OBS Help Guide can be found at its [original site](https://drive.google.com/file/d/0B_jVUZcoOfCGR0VMSEpQTElYTDQ/view), or read directly from a [local copy made available in the docs folder](./docs/OBS Studio Help Guide 16-05-2016.pdf).
 * **[Qt](https://www.qt.io/)**. The tool is built in C++ with the Qt library in order to be easily portable.
 * **[CMake](https://cmake.org/)**. The CMake tool is used to easily configure the build environment in different platforms and generate the proper Visual Studio project or make file for building.

## Building Notes

- The translations are handled by the CMake configuration process via a target called "Translations" and a configuration option named `CREATE_TRANSLATIONS`. If that option is turned on during configuration, when that target is built the translation files (`*.ts`) will be either created (if not existing) or updated (with new strings found in the source files and marked with a call to `tr`), and then the binary translation files (`*.qm`) will be compiled. If that option is turned off during configuration, when that target is built the existing translation files will simply be used to compile the binary translation files (i.e. no translation update will be performed).

>> A **very important** remark is that if the CREATE_TRANSLATIONS is turned on during configuration/generation time, the clean of the "Translations" target (either by issuing `make clean` or cleaning the project on Visual Studio) during build time will **delete** the translation files in the source directory!

## Copyright Notes

- The Brazil and United Kingdom flags used in the application are from the website [Free Flag Icons website](http://www.freeflagicons.com/).