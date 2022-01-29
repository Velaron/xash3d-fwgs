cmake_minimum_required(VERSION 2.8.12)
project(public)

set(XASH_PUBLIC public)

file(GLOB XASH_PUBLIC_SRC "${ROOT_SOURCE_DIRECTORY}/public/*.c")

add_library(${XASH_PUBLIC} STATIC ${XASH_PUBLIC_SRC})

target_include_directories(
	${XASH_PUBLIC} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/common"
	"${ROOT_SOURCE_DIRECTORY}/engine"
	"${ROOT_SOURCE_DIRECTORY}/public"
)