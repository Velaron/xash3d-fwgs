cmake_minimum_required(VERSION 2.8.12)
project(REF_GL)

option(GL_STATIC "Link to OpenGL directly." OFF)

file(GLOB REF_GL_SRC "*.c")

include_directories(
	"../common"
	"../engine"
	"../engine/common"
	"../engine/server"
	"../engine/client"
	"../engine/client/avi"
	"../pm_shared"
	"../public"
	"../filesystem"
)

if(ANDROID)
	# nanogl
	add_subdirectory(nanogl)

	# ref_gles1
	set(REF_GLES1_LIB ref_gles1)

	add_library(${REF_GLES1_LIB} MODULE ${REF_GL_SRC})
	
	target_compile_definitions(${REF_GLES1_LIB} PRIVATE XASH_NANOGL REF_DLL)

	target_link_libraries(${REF_GLES1_LIB} public nanogl log)

	install(TARGETS ${REF_GLES1_LIB} DESTINATION .)

	# gl-wes-v2
	add_subdirectory(gl-wes-v2)

	# ref_gles2
	set(REF_GLES2_LIB ref_gles2)

	add_library(${REF_GLES2_LIB} MODULE ${REF_GL_SRC})

	target_compile_definitions(${REF_GLES2_LIB} PRIVATE XASH_WES REF_DLL)

	target_link_libraries(${REF_GLES2_LIB} public gl-wes-v2 log)

	install(TARGETS ${REF_GLES2_LIB} DESTINATION .)

	# gl4es
#	set(XASH_GL4ES gl4es)

#	file(GLOB XASH_GL4ES_SRC
#		"../ref_gl/gl4es/src/gl/*.c"
#		"../ref_gl/gl4es/src/gl/math/*.c"
#		"../ref_gl/gl4es/src/gl/wrap/*.c"
#		"../ref_gl/gl4es/src/glx/hardext.c"
#	)

#	add_library(${XASH_GL4ES} STATIC ${XASH_GL4ES_SRC})

#	target_compile_definitions(
#		${XASH_GL4ES} PRIVATE
#		NOX11 NO_GBM NO_INIT_CONSTRUCTOR
#		DEFAULT_ES=2 NOEGL
#		EXTERNAL_GETPROCADDRESS=GL4ES_GetProcAddress
#		NO_LOADER STATICLIB
#	)
	
#	target_link_libraries(${XASH_GL4ES} public)

#	target_include_directories(${XASH_GL4ES} PRIVATE
#		"../ref_gl/gl4es/src"
#		"../ref_gl/gl4es/include"
#		"../ref_gl/gl4es/src/gl"
#		"../ref_gl/gl4es/src/glx"
#	)

	# ref_gl4es
#	set(XASH_REF_GL4ES ref_gl4es)

#	add_library(${XASH_REF_GL4ES} MODULE ${XASH_REF_GL_SRC})

#	target_compile_definitions(${XASH_REF_GL4ES} PRIVATE XASH_GL4ES XASH_GL_STATIC REF_DLL)

#	target_link_libraries(${XASH_REF_GL4ES} public gl4es log)

#	install(TARGETS ${XASH_REF_GL4ES} DESTINATION .)
else()
	set(REF_GL_LIB ref_gl)

	add_library(${REF_GL_LIB} MODULE ${REF_GL_SRC})

	target_compile_definitions(${REF_GL_LIB} PRIVATE REF_DLL)

	target_link_libraries(${REF_GL_LIB} public)

	install(TARGETS ${REF_GL_LIB} DESTINATION .)

	if(MSVC)
		install(FILES $<TARGET_PDB_FILE:${REF_GL_LIB}> DESTINATION . OPTIONAL)
	endif()
endif()