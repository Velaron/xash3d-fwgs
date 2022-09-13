cmake_minimum_required(VERSION 2.8.12)
project(PUBLIC)

set(PUBLIC_LIB public)

file(GLOB PUBLIC_SRC "*.c")

add_library(${PUBLIC_LIB} STATIC ${PUBLIC_SRC})

target_include_directories(
	${PUBLIC_LIB} PRIVATE
	"../common"
	"../engine"
	"."
)