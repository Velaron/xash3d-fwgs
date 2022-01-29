cmake_minimum_required(VERSION 2.8.12)
project(xash3d-fwgs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
include(CheckIncludeFile)

option(XASH_DEDICATED "Build dedicated server only." OFF)

if(CMAKE_SIZEOF_VOID_P MATCHES "8")
	option(XASH_64BIT "Enable experimental 64-bit build. Note that you must also provide 64-bit game binaries." ON)
else()
	option(XASH_64BIT "Enable experimental 64-bit build. Note that you must also provide 64-bit game binaries." OFF)
endif()

if(XASH_DEDICATED) 
	option(XASH_SDL "Enable SDL." OFF)
	option(XASH_SINGLE_BINARY "Build the game launcher and the engine as a single executable." ON)
else()
	option(XASH_SDL "Enable SDL." ON)
	option(XASH_SINGLE_BINARY "Build the game launcher and the engine as a single executable." OFF)
endif()

option(XASH_BSP2 "Enable BSP2 support." OFF)
set(XASH_LOW_MEMORY "0" CACHE STRING "Low memory mode (0-2).")

check_include_file("stdint.h" STDINT_H_EXISTS)
if(STDINT_H_EXISTS)
	add_definitions(-DSTDINT_H="stdint.h")
else()
	add_definitions(-DSTDINT_H="pstdint.h")
endif()

if(XASH_BSP2)
	add_definitions(-DSUPPORT_BSP2_FORMAT)
endif()

if(XASH_LOW_MEMORY)
	add_definitions(-DXASH_LOW_MEMORY=${XASH_LOW_MEMORY})
endif()

# hack
set(ROOT_SOURCE_DIRECTORY "../../..")

add_subdirectory(public)

if(NOT XASH_DEDICATED)
	add_subdirectory(ref_gl)
	add_subdirectory(ref_soft)

	SET(MAINUI_NAME "menu" CACHE STRING "Force menu library name.")

	add_subdirectory(
		${CMAKE_CURRENT_SOURCE_DIR}/../../mainui
		${CMAKE_CURRENT_BINARY_DIR}/mainui
	)

	if(NOT XASH_64BIT AND NOT ANDROID)
		add_subdirectory(vgui_support)
	endif()
endif()

if(NOT XASH_SINGLE_BINARY)
	add_subdirectory(game_launch)
endif()

add_subdirectory(engine)