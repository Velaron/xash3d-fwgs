cmake_minimum_required(VERSION 3.1)
project(public)

set(XASH_PUBLIC public)
file(GLOB XASH_PUBLIC_SRC "${PROJECT_SOURCE_DIR}/*.c")
add_library(${XASH_PUBLIC} STATIC ${XASH_PUBLIC_SRC})
target_include_directories(
	${XASH_PUBLIC} PRIVATE
	"${PROJECT_SOURCE_DIR}/../common"
	"${PROJECT_SOURCE_DIR}/../engine"
	"${PROJECT_SOURCE_DIR}"
)