cmake_minimum_required(VERSION 2.6)

set(PLUGIN "colorpicker")

set(HEADERS
	colorpicker.h
)

set(SOURCES
	colorpicker.cpp
)

set(MOCS
	colorpicker.h
)

set(UIS
)

set(LIBRARIES
  ${LXQT_LIBRARIES}
)

#*******************************************
include ("../BuildPlugin.cmake")
BUILD_LXQT_PLUGIN(${PLUGIN})
