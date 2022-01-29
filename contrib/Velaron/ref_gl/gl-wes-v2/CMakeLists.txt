cmake_minimum_required(VERSION 2.8.12)
project(gl-wes-v2)

# hack
set(ROOT_SOURCE_DIRECTORY "../../../..")

set(XASH_GL_WES gl-wes-v2)

file(GLOB XASH_GL_WES_SRC "${ROOT_SOURCE_DIRECTORY}/ref_gl/gl-wes-v2/src/*.c")

add_library(${XASH_GL_WES} STATIC ${XASH_GL_WES_SRC})

target_compile_definitions(${XASH_GL_WES} PRIVATE REF_DLL WES_MANGLE_PREPEND)

target_include_directories(${XASH_GL_WES} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/ref_gl/gl-wes-v2/src")