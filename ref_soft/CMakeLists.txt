cmake_minimum_required(VERSION 3.1)
project(ref_soft)

set(XASH_REF_SOFT ref_soft)

file(GLOB XASH_REF_SOFT_SRC "${PROJECT_SOURCE_DIR}/*.c")
add_library(${XASH_REF_SOFT} MODULE ${XASH_REF_SOFT_SRC})
target_include_directories(
	${XASH_REF_SOFT} PRIVATE
	"${PROJECT_SOURCE_DIR}/../common"
	"${PROJECT_SOURCE_DIR}/../engine"
	"${PROJECT_SOURCE_DIR}/../engine/common"
	"${PROJECT_SOURCE_DIR}/../engine/server"
	"${PROJECT_SOURCE_DIR}/../engine/client"
	"${PROJECT_SOURCE_DIR}/../engine/client/avi"
	"${PROJECT_SOURCE_DIR}/../pm_shared"
	"${PROJECT_SOURCE_DIR}/../public"
	"${PROJECT_SOURCE_DIR}"
)
target_compile_definitions(${XASH_REF_SOFT} PRIVATE REF_DLL)
target_link_libraries(${XASH_REF_SOFT} public)

install(TARGETS ${XASH_REF_SOFT} DESTINATION .)
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${XASH_REF_SOFT}> DESTINATION . OPTIONAL)
endif()