cmake_minimum_required(VERSION 3.1)
project(vgui_support)

set(XASH_VGUI_SUPPORT vgui_support)

file(GLOB XASH_VGUI_SUPPORT_SRC "${PROJECT_SOURCE_DIR}/*.cpp")
find_library(
	VGUI_LIBRARY vgui
	HINTS "${PROJECT_SOURCE_DIR}/../vgui-dev"
	PATH_SUFFIXES lib lib/win32_vc6
)
add_library(${XASH_VGUI_SUPPORT} MODULE ${XASH_VGUI_SUPPORT_SRC})
target_include_directories(
	${XASH_VGUI_SUPPORT} PRIVATE
	"${PROJECT_SOURCE_DIR}/../common"
	"${PROJECT_SOURCE_DIR}/../engine"
	"${PROJECT_SOURCE_DIR}/../vgui-dev/include"
	"${PROJECT_SOURCE_DIR}"
)
target_link_libraries(${XASH_VGUI_SUPPORT} ${VGUI_LIBRARY})

install(TARGETS ${XASH_VGUI_SUPPORT} DESTINATION .)
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${XASH_VGUI_SUPPORT}> DESTINATION . OPTIONAL)
endif()