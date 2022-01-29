cmake_minimum_required(VERSION 2.8.12)
project(ref_gl)

option(XASH_GL_STATIC "Link to OpenGL directly." OFF)

file(GLOB XASH_REF_GL_SRC "${ROOT_SOURCE_DIRECTORY}/ref_gl/*.c")

include_directories(
	"${ROOT_SOURCE_DIRECTORY}/common"
	"${ROOT_SOURCE_DIRECTORY}/engine"
	"${ROOT_SOURCE_DIRECTORY}/engine/common"
	"${ROOT_SOURCE_DIRECTORY}/engine/server"
	"${ROOT_SOURCE_DIRECTORY}/engine/client"
	"${ROOT_SOURCE_DIRECTORY}/engine/client/avi"
	"${ROOT_SOURCE_DIRECTORY}/pm_shared"
	"${ROOT_SOURCE_DIRECTORY}/public"
	"${ROOT_SOURCE_DIRECTORY}/ref_gl"
)

if(ANDROID)
	# nanogl
	add_subdirectory(nanogl)

	# ref_gles1
	set(XASH_REF_GLES1 ref_gles1)

	add_library(${XASH_REF_GLES1} MODULE ${XASH_REF_GL_SRC})
	
	target_compile_definitions(${XASH_REF_GLES1} PRIVATE XASH_NANOGL REF_DLL)

	target_link_libraries(${XASH_REF_GLES1} public nanogl log)

	install(TARGETS ${XASH_REF_GLES1} DESTINATION .)

	# gl-wes-v2
	add_subdirectory(gl-wes-v2)

	# ref_gles2
	set(XASH_REF_GLES2 ref_gles2)

	add_library(${XASH_REF_GLES2} MODULE ${XASH_REF_GL_SRC})

	target_compile_definitions(${XASH_REF_GLES2} PRIVATE XASH_WES REF_DLL)

	target_link_libraries(${XASH_REF_GLES2} public gl-wes-v2 log)

	install(TARGETS ${XASH_REF_GLES2} DESTINATION .)

	# gl4es
	set(XASH_GL4ES gl4es)

	file(GLOB XASH_GL4ES_SRC
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src/gl/*.c"
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src/gl/math/*.c"
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src/gl/wrap/*.c"
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src/glx/hardext.c"
	)

	add_library(${XASH_GL4ES} STATIC ${XASH_GL4ES_SRC})

	target_compile_definitions(
		${XASH_GL4ES} PRIVATE
		NOX11 NO_GBM NO_INIT_CONSTRUCTOR
		DEFAULT_ES=2 NOEGL
		EXTERNAL_GETPROCADDRESS=GL4ES_GetProcAddress
		NO_LOADER STATICLIB
	)
	
	target_link_libraries(${XASH_GL4ES} public)

	target_include_directories(${XASH_GL4ES} PRIVATE
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src"
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/include"
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src/gl"
		"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl4es/src/glx"
	)

	# ref_gl4es
	set(XASH_REF_GL4ES ref_gl4es)

	add_library(${XASH_REF_GL4ES} MODULE ${XASH_REF_GL_SRC})

	target_compile_definitions(${XASH_REF_GL4ES} PRIVATE XASH_GL4ES XASH_GL_STATIC REF_DLL)

	target_link_libraries(${XASH_REF_GL4ES} public gl4es log)

	install(TARGETS ${XASH_REF_GL4ES} DESTINATION .)
else()
	# ref_gl
	set(XASH_REF_GL ref_gl)

	add_library(${XASH_REF_GL} MODULE ${XASH_REF_GL_SRC})

	target_compile_definitions(${XASH_REF_GL} PRIVATE REF_DLL)

	target_link_libraries(${XASH_REF_GL} public)

	install(TARGETS ${XASH_REF_GL} DESTINATION .)

	if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
		install(FILES $<TARGET_PDB_FILE:${XASH_REF_GL}> DESTINATION . OPTIONAL)
	endif()
endif()