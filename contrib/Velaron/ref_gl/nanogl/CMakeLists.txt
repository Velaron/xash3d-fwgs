cmake_minimum_required(VERSION 2.8.12)
project(nanogl)

# hack
set(ROOT_SOURCE_DIRECTORY "../../../..")

set(XASH_NANOGL nanogl)

file(GLOB XASH_NANOGL_SRC "${ROOT_SOURCE_DIRECTORY}/ref_gl/nanogl/*.cpp")

add_library(${XASH_NANOGL} STATIC ${XASH_NANOGL_SRC})

target_compile_definitions(${XASH_NANOGL} PRIVATE __MULTITEXTURE_SUPPORT__ NANOGL_MANGLE_PREPEND)

target_include_directories(
	${XASH_NANOGL} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/ref_gl/nanogl"
	"${ROOT_SOURCE_DIRECTORY}/ref_gl/nanogl/GL"
)