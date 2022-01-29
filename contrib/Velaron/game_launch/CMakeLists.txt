cmake_minimum_required(VERSION 2.8.12)
project(game_launch)

set(XASH_GAME_LAUNCH xash3d)

add_executable(
	${XASH_GAME_LAUNCH} WIN32
	"${ROOT_SOURCE_DIRECTORY}/game_launch/game.cpp"
	"${ROOT_SOURCE_DIRECTORY}/game_launch/game.rc"
)

target_include_directories(
	${XASH_GAME_LAUNCH} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/common"
	"${ROOT_SOURCE_DIRECTORY}/public"
)

install(TARGETS ${XASH_GAME_LAUNCH} DESTINATION .)

if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	install(FILES $<TARGET_PDB_FILE:${XASH_GAME_LAUNCH}> DESTINATION . OPTIONAL)
endif()