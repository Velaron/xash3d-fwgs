cmake_minimum_required(VERSION 2.8.12)
project(ref_soft)

set(XASH_REF_SOFT ref_soft)

file(GLOB XASH_REF_SOFT_SRC "${ROOT_SOURCE_DIRECTORY}/ref_soft/*.c")

add_library(${XASH_REF_SOFT} MODULE ${XASH_REF_SOFT_SRC})

target_include_directories(
	${XASH_REF_SOFT} PRIVATE
	"${ROOT_SOURCE_DIRECTORY}/common"
	"${ROOT_SOURCE_DIRECTORY}/engine"
	"${ROOT_SOURCE_DIRECTORY}/engine/common"
	"${ROOT_SOURCE_DIRECTORY}/engine/server"
	"${ROOT_SOURCE_DIRECTORY}/engine/client"
	"${ROOT_SOURCE_DIRECTORY}/engine/client/avi"
	"${ROOT_SOURCE_DIRECTORY}/pm_shared"
	"${ROOT_SOURCE_DIRECTORY}/public"
	"${ROOT_SOURCE_DIRECTORY}/ref_soft"
)

target_compile_definitions(${XASH_REF_SOFT} PRIVATE REF_DLL)

target_link_libraries(${XASH_REF_SOFT} public)

install(TARGETS ${XASH_REF_SOFT} DESTINATION .)

if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	install(FILES $<TARGET_PDB_FILE:${XASH_REF_SOFT}> DESTINATION . OPTIONAL)
endif()