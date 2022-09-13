cmake_minimum_required(VERSION 2.8.12)
project(GAME_LAUNCH)

set(GAME_LAUNCH_EXE xash3d)

add_executable(
	${GAME_LAUNCH_EXE} WIN32 "game.cpp" "game.rc"
)

target_include_directories(
	${GAME_LAUNCH_EXE} PRIVATE
	"../common"
	"../public"
)

install(TARGETS ${GAME_LAUNCH_EXE} DESTINATION .)

if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${GAME_LAUNCH_EXE}> DESTINATION . OPTIONAL)
endif()