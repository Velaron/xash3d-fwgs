cmake_minimum_required(VERSION 2.8.12)
project(REF_SOFT)

set(REF_SOFT_LIB ref_soft)

file(GLOB REF_SOFT_SRC "*.c")

add_library(${REF_SOFT_LIB} MODULE ${REF_SOFT_SRC})

target_include_directories(
	${REF_SOFT_LIB} PRIVATE
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

target_compile_definitions(${REF_SOFT_LIB} PRIVATE REF_DLL)

target_link_libraries(${REF_SOFT_LIB} public)

install(TARGETS ${REF_SOFT_LIB} DESTINATION .)

if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${REF_SOFT_LIB}> DESTINATION . OPTIONAL)
endif()