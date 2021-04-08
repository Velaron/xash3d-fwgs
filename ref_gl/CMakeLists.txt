cmake_minimum_required(VERSION 3.1)
project(ref_gl)

# ref_gl
set(XASH_REF_GL ref_gl)
file(GLOB XASH_REF_GL_SRC "${PROJECT_SOURCE_DIR}/*.c")
list(
	APPEND XASH_REF_GL_INC
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
add_library(${XASH_REF_GL} MODULE ${XASH_REF_GL_SRC})
target_include_directories(${XASH_REF_GL} PRIVATE ${XASH_REF_GL_INC})
target_compile_definitions(${XASH_REF_GL} PRIVATE REF_DLL)
target_link_libraries(${XASH_REF_GL} public)
install(TARGETS ${XASH_REF_GL} DESTINATION .)
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${XASH_REF_GL}> DESTINATION . OPTIONAL)
endif()

if(ANDROID)
# nanogl
add_subdirectory(nanogl)

# ref_gles1
set(XASH_REF_GLES1 ref_gles1)

add_library(${XASH_REF_GLES1} MODULE ${XASH_REF_GL_SRC})
target_include_directories(${XASH_REF_GLES1} PRIVATE ${XASH_REF_GL_INC})
target_compile_definitions(${XASH_REF_GLES1} PRIVATE XASH_NANOGL REF_DLL)
target_link_libraries(${XASH_REF_GLES1} public nanogl log)
install(TARGETS ${XASH_REF_GLES1} DESTINATION .)

# gl-wes-v2
add_subdirectory(gl-wes-v2)

# ref_gles2
set(XASH_REF_GLES2 ref_gles2)

add_library(${XASH_REF_GLES2} MODULE ${XASH_REF_GL_SRC})
target_include_directories(${XASH_REF_GLES2} PRIVATE ${XASH_REF_GL_INC})
target_compile_definitions(${XASH_REF_GLES2} PRIVATE XASH_WES REF_DLL)
target_link_libraries(${XASH_REF_GLES2} public gl-wes-v2 log)
install(TARGETS ${XASH_REF_GLES2} DESTINATION .)
endif()