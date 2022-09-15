cmake_minimum_required(VERSION 2.8.12)
project(ENGINE)

set(ENGINE_LIB xash)

file(
	GLOB ENGINE_SRC
	"common/*.c"
	"common/imagelib/*.c"
	"common/soundlib/*.c"
	"common/soundlib/libmpg/*.c"
	"server/*.c"
)

file(
	GLOB ENGINE_CLIENT_SRC
	"client/*.c"
	"client/avi/*.c"
	"client/vgui/*.c"
)

file(
	GLOB ENGINE_WIN32_SRC
	"platform/win32/*.c"
)

file(
	GLOB ENGINE_SDL_SRC
	"platform/sdl/*.c"
)

file(
	GLOB ENGINE_ANDROID_SRC
	"platform/posix/*.c"
	"platform/linux/*.c"
	"platform/android/*.c"
	"platform/android/*.cpp"
)

if(ANDROID)
	add_library(${ENGINE_LIB} MODULE ${ENGINE_SRC})
	target_compile_definitions(${ENGINE_LIB} PRIVATE SINGLE_BINARY DEFAULT_DEV=2 DEFAULT_LOG=1)
elseif(SINGLE_BINARY)
	add_executable(${ENGINE_LIB} WIN32 ${ENGINE_SRC})
	set_target_properties(${ENGINE_LIB} PROPERTIES OUTPUT_NAME "xash3d")
	target_compile_definitions(${ENGINE_LIB} PRIVATE SINGLE_BINARY)
else()
	add_library(${ENGINE_LIB} MODULE ${ENGINE_SRC})
endif()

if(WIN32)
	target_sources(${ENGINE_LIB} PRIVATE ${ENGINE_WIN32_SRC})
	target_compile_definitions(${ENGINE_LIB} PRIVATE DBGHELP PSAPI_VERSION=1)
	target_link_libraries(${ENGINE_LIB} dbghelp psapi ws2_32)
endif()

if(ANDROID)
	target_sources(${ENGINE_LIB} PRIVATE ${ENGINE_ANDROID_SRC})
	target_compile_definitions(${ENGINE_LIB} PRIVATE XASH_SDLMAIN)
	target_link_libraries(${ENGINE_LIB} log)
endif()

if(DEDICATED)
	target_compile_definitions(${ENGINE_LIB} PRIVATE XASH_DEDICATED)
else()
	target_sources(${ENGINE_LIB} PRIVATE ${ENGINE_CLIENT_SRC})
endif()

if(NOT DEDICATED)
	if(NOT ANDROID)
		set(SDL2_BUILDING_LIBRARY ${SINGLE_BINARY})
		find_package(SDL2)
	endif()

	target_sources(${ENGINE_LIB} PRIVATE ${ENGINE_SDL_SRC})
	target_compile_definitions(${ENGINE_LIB} PRIVATE XASH_SDL=2)

	if(NOT ANDROID)
		target_include_directories(${ENGINE_LIB} PRIVATE ${SDL2_INCLUDE_DIR})
		target_link_libraries(${ENGINE_LIB} ${SDL2_LIBRARY} opus)
	else()
		target_include_directories(${ENGINE_LIB} PRIVATE "../../SDL/include")
		target_link_libraries(${ENGINE_LIB} SDL2 opus)
	endif()
endif()

target_link_libraries(${ENGINE_LIB} public)

target_include_directories(
	${ENGINE_LIB} PRIVATE
	"../common"
	"../pm_shared"
	"../public"
	"common"
	"common/imagelib"
	"common/soundlib"
	"client"
	"client/vgui"
	"server"
	"../filesystem"
	"../3rdparty/opus/opus/include"
	"."
)

install(TARGETS ${ENGINE_LIB} DESTINATION .)

if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${ENGINE_LIB}> DESTINATION . OPTIONAL)
endif()
