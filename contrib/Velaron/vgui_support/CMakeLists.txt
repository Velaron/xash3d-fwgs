cmake_minimum_required(VERSION 2.8.12)
project(vgui_support)

set(XASH_VGUI_SUPPORT vgui_support)

file(GLOB XASH_VGUI_SUPPORT_SRC "${ROOT_SOURCE_DIRECTORY}/vgui_support/*.cpp")

find_library(
	VGUI_LIBRARY vgui
	HINTS "${ROOT_SOURCE_DIRECTORY}/vgui-dev"
	PATH_SUFFIXES lib lib/win32_vc6
)

add_library(${XASH_VGUI_SUPPORT} MODULE ${XASH_VGUI_SUPPORT_SRC})

target_include_directories(
	${XASH_VGUI_SUPPORT} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/common"
	"${ROOT_SOURCE_DIRECTORY}/engine"
	"${ROOT_SOURCE_DIRECTORY}/vgui-dev/include"
	"${ROOT_SOURCE_DIRECTORY}/public"
	"${ROOT_SOURCE_DIRECTORY}/vgui_support"
)

target_link_libraries(${XASH_VGUI_SUPPORT} ${VGUI_LIBRARY})

install(TARGETS ${XASH_VGUI_SUPPORT} DESTINATION .)

if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	install(FILES $<TARGET_PDB_FILE:${XASH_VGUI_SUPPORT}> DESTINATION . OPTIONAL)
endif()
