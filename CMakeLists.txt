cmake_minimum_required(VERSION 3.1)
project(xash3d-fwgs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
include(CheckIncludeFile)
set(XASH_VERSION 0.20)

option(XASH_DEDICATED "Enable dedicated build. Servers must choose this." OFF)
#set(SDL2_PATH "${XASH_SRC_DIR}/SDL2" CACHE PATH "Custom SDL2 library path.")

set(GAME_DIR "cstrike" CACHE STRING "Game directory name.")
set(CLIENT_INSTALL_DIR "cl_dlls" CACHE STRING "Client library location.")
set(SERVER_INSTALL_DIR "dlls" CACHE STRING "Server library location.")

if(CMAKE_SIZEOF_VOID_P MATCHES "8")
	option(XASH_64BIT "Enable experimental 64-bit build. Note that you must also provide 64-bit game binaries." ON)
else()
	option(XASH_64BIT "Enable experimental 64-bit build. Note that you must also provide 64-bit game binaries." OFF)
endif()

if(XASH_DEDICATED) 
	option(XASH_SDL "Enable SDL." OFF)
	option(XASH_SINGLE_BINARY "Don't build game launcher and build engine as executable." ON)
else()
	option(XASH_SDL "Enable SDL." ON)
	option(XASH_SINGLE_BINARY "Don't build game launcher and build engine as executable." OFF)
endif()

check_include_file("stdint.h" STDINT_H_EXISTS)
if(STDINT_H_EXISTS)
    add_definitions(-DSTDINT_H="stdint.h")
else()
	add_definitions(-DSTDINT_H="pstdint.h")
endif()

add_subdirectory(public)

if(NOT XASH_DEDICATED)
	add_subdirectory(ref_gl)
	add_subdirectory(ref_soft)

	if(NOT XASH_64BIT AND NOT ANDROID)
		add_subdirectory(vgui_support)
	endif()
endif()

if(NOT XASH_SINGLE_BINARY)
	add_subdirectory(game_launch)
endif()

add_subdirectory(engine)