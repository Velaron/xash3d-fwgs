cmake_minimum_required(VERSION 3.1)
project(game_launch)

set(XASH_GAME_LAUNCH xash3d)
add_executable(
	${XASH_GAME_LAUNCH} WIN32
	"${PROJECT_SOURCE_DIR}/game.cpp"
	"${PROJECT_SOURCE_DIR}/game.rc"
)
target_include_directories(
	${XASH_GAME_LAUNCH} PRIVATE
	"${PROJECT_SOURCE_DIR}/../common"
	"${PROJECT_SOURCE_DIR}/../public"
)

install(TARGETS ${XASH_GAME_LAUNCH} DESTINATION .)
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${XASH_GAME_LAUNCH}> DESTINATION . OPTIONAL)
endif()