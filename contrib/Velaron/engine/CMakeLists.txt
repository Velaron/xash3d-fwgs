cmake_minimum_required(VERSION 2.8.12)
project(engine)

set(XASH_ENGINE xash)

file(
	GLOB XASH_ENGINE_SRC
	"${ROOT_SOURCE_DIRECTORY}/engine/common/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/common/imagelib/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/common/soundlib/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/common/soundlib/libmpg/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/server/*.c"
)

file(
	GLOB XASH_ENGINE_CLIENT_SRC
	"${ROOT_SOURCE_DIRECTORY}/engine/client/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/client/avi/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/client/vgui/*.c"
)

file(
	GLOB XASH_ENGINE_WIN32_SRC
	"${ROOT_SOURCE_DIRECTORY}/engine/platform/win32/*.c"
)

file(
	GLOB XASH_ENGINE_SDL_SRC
	"${ROOT_SOURCE_DIRECTORY}/engine/platform/sdl/*.c"
)

file(
	GLOB XASH_ENGINE_ANDROID_SRC
	"${ROOT_SOURCE_DIRECTORY}/engine/platform/posix/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/platform/linux/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/platform/android/*.c"
	"${ROOT_SOURCE_DIRECTORY}/engine/platform/android/*.cpp"
)

if(XASH_SINGLE_BINARY)
	add_executable(${XASH_ENGINE} WIN32 ${XASH_ENGINE_SRC})
	set_target_properties(${XASH_ENGINE} PROPERTIES OUTPUT_NAME "xash3d")
	target_compile_definitions(${XASH_ENGINE} PRIVATE SINGLE_BINARY)
else()
	add_library(${XASH_ENGINE} MODULE ${XASH_ENGINE_SRC})
endif()

if(WIN32)
	target_sources(${XASH_ENGINE} PRIVATE ${XASH_ENGINE_WIN32_SRC})
	target_compile_definitions(${XASH_ENGINE} PRIVATE DBGHELP PSAPI_VERSION=1)
	target_link_libraries(${XASH_ENGINE} dbghelp psapi ws2_32)
endif()

if(ANDROID)
	target_sources(${XASH_ENGINE} PRIVATE ${XASH_ENGINE_ANDROID_SRC})
	target_link_libraries(${XASH_ENGINE} log)
	set(XASH_SDL OFF)
endif()

if(XASH_DEDICATED)
	target_compile_definitions(${XASH_ENGINE} PRIVATE XASH_DEDICATED)
else()
	target_sources(${XASH_ENGINE} PRIVATE ${XASH_ENGINE_CLIENT_SRC})
endif()

if(XASH_SDL)
	set(SDL2_BUILDING_LIBRARY ${XASH_SINGLE_BINARY})
	find_package(SDL2)
	target_sources(${XASH_ENGINE} PRIVATE ${XASH_ENGINE_SDL_SRC})
	target_compile_definitions(${XASH_ENGINE} PRIVATE XASH_SDL=2)
	target_include_directories(${XASH_ENGINE} PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(${XASH_ENGINE} ${SDL2_LIBRARY})
endif()

target_link_libraries(${XASH_ENGINE} public)

target_include_directories(
	${XASH_ENGINE} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/engine"
	"${ROOT_SOURCE_DIRECTORY}/common"
	"${ROOT_SOURCE_DIRECTORY}/pm_shared"
	"${ROOT_SOURCE_DIRECTORY}/public"
	"${ROOT_SOURCE_DIRECTORY}/engine/common"
	"${ROOT_SOURCE_DIRECTORY}/engine/common/imagelib"
	"${ROOT_SOURCE_DIRECTORY}/engine/common/soundlib"
	"${ROOT_SOURCE_DIRECTORY}/engine/client"
	"${ROOT_SOURCE_DIRECTORY}/engine/client/vgui"
	"${ROOT_SOURCE_DIRECTORY}/engine/server"
)

install(TARGETS ${XASH_ENGINE} DESTINATION .)

if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	install(FILES $<TARGET_PDB_FILE:${XASH_ENGINE}> DESTINATION . OPTIONAL)
endif()