cmake_minimum_required(VERSION 2.8.12)
project(XASH3D-FWGS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
include(CheckIncludeFile)

option(DEDICATED "Build dedicated server only." OFF)

if(CMAKE_SIZEOF_VOID_P MATCHES "8")
#	option(XASH_64BIT "Enable experimental 64-bit build. Note that you must also provide 64-bit game binaries." ON)
else()
#	option(XASH_64BIT "Enable experimental 64-bit build. Note that you must also provide 64-bit game binaries." OFF)
endif()

if(DEDICATED OR ANDROID) 
#	option(XASH_SDL "Enable SDL." OFF)
	option(SINGLE_BINARY "Build the game launcher and the engine as a single executable." ON)
else()
#	option(XASH_SDL "Enable SDL." ON)
	option(SINGLE_BINARY "Build the game launcher and the engine as a single executable." OFF)
endif()

option(BSP2 "Enable BSP2 support." OFF)
set(LOW_MEMORY "0" CACHE STRING "Low memory mode (0-2).")

check_include_file("stdint.h" STDINT_H_EXISTS)
if(STDINT_H_EXISTS)
	add_definitions(-DSTDINT_H="stdint.h")
else()
	add_definitions(-DSTDINT_H="pstdint.h")
endif()

check_include_file("alloca.h" ALLOCA_H_EXISTS)
if(ALLOCA_H_EXISTS)
	add_definitions(-DALLOCA_H="alloca.h")
else()
	add_definitions(-DALLOCA_H="malloc.h")
endif()

if(BSP2)
	add_definitions(-DSUPPORT_BSP2_FORMAT)
endif()

if(LOW_MEMORY)
	add_definitions(-DXASH_LOW_MEMORY=${LOW_MEMORY})
endif()

add_subdirectory(public)

if(NOT DEDICATED)
	add_subdirectory(ref_gl)
	add_subdirectory(ref_soft)

#	SET(MAINUI_NAME "menu" CACHE STRING "Force menu library name.")

	add_subdirectory(mainui)

#	if(NOT XASH_64BIT AND NOT ANDROID)
#		add_subdirectory(vgui_support)
#	endif()

	set(OPUS_USE_ALLOCA ON CACHE BOOL "" FORCE)
	set(OPUS_CUSTOM_MODES ON CACHE BOOL "" FORCE)
	set(FLOAT_APPROX ON CACHE BOOL "" FORCE)
	add_subdirectory(3rdparty/opus/opus)
endif()

if(NOT SINGLE_BINARY AND NOT ANDROID)
	add_subdirectory(game_launch)
endif()

add_subdirectory(filesystem)
add_subdirectory(engine)