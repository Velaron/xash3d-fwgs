cmake_minimum_required(VERSION 2.8.12)
project(FILESYSTEM)

set(FILESYSTEM_LIB filesystem_stdio)

file(GLOB FILESYSTEM_SRC "*.c" "*.cpp")

add_library(${FILESYSTEM_LIB} MODULE ${FILESYSTEM_SRC})

if(ANDROID)
	target_link_libraries(${FILESYSTEM_LIB} SDL2 public)
else()
	target_link_libraries(${FILESYSTEM_LIB} public)
endif()

target_include_directories(
	${FILESYSTEM_LIB} PRIVATE
	"../common"
	"../engine"
	"../public"
)

install(TARGETS ${FILESYSTEM_LIB} DESTINATION .)

if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${FILESYSTEM_LIB}> DESTINATION . OPTIONAL)
endif()
