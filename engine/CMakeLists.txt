cmake_minimum_required(VERSION 3.1)
project(engine)

set(XASH_ENGINE xash)

file(
	GLOB XASH_ENGINE_SRC
	"${PROJECT_SOURCE_DIR}/common/*.c"
	"${PROJECT_SOURCE_DIR}/common/imagelib/*.c"
	"${PROJECT_SOURCE_DIR}/common/soundlib/*.c"
	"${PROJECT_SOURCE_DIR}/common/soundlib/libmpg/*.c"
	"${PROJECT_SOURCE_DIR}/server/*.c"
)

file(
	GLOB XASH_ENGINE_CLIENT_SRC
	"${PROJECT_SOURCE_DIR}/client/*.c"
	"${PROJECT_SOURCE_DIR}/client/avi/*.c"
	"${PROJECT_SOURCE_DIR}/client/vgui/*.c"
)

file(
	GLOB XASH_ENGINE_WIN32_SRC
	"${PROJECT_SOURCE_DIR}/platform/win32/*.c"
)

file(
	GLOB XASH_ENGINE_SDL_SRC
	"${PROJECT_SOURCE_DIR}/platform/sdl/*.c"
)

file(
	GLOB XASH_ENGINE_ANDROID_SRC
	"${PROJECT_SOURCE_DIR}/platform/posix/*.c"
	"${PROJECT_SOURCE_DIR}/platform/linux/*.c"
	"${PROJECT_SOURCE_DIR}/platform/android/*.c"
	"${PROJECT_SOURCE_DIR}/platform/android/*.cpp"
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
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/.."
	"${PROJECT_SOURCE_DIR}/../common"
	"${PROJECT_SOURCE_DIR}/../pm_shared"
	"${PROJECT_SOURCE_DIR}/../public"
	"${PROJECT_SOURCE_DIR}/common"
	"${PROJECT_SOURCE_DIR}/common/imagelib"
	"${PROJECT_SOURCE_DIR}/common/soundlib"
	"${PROJECT_SOURCE_DIR}/client"
	"${PROJECT_SOURCE_DIR}/client/vgui"
	"${PROJECT_SOURCE_DIR}/server"
)

install(TARGETS ${XASH_ENGINE} DESTINATION .)
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${XASH_ENGINE}> DESTINATION . OPTIONAL)
endif()