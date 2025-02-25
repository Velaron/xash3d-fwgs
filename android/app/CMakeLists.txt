cmake_minimum_required(VERSION 3.22.1)

# Only used to build Android project

project(xash3d-fwgs-android)

# armeabi-v7a requires cpufeatures library
include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()

find_package(PythonInterp 2.7 REQUIRED)

get_filename_component(C_COMPILER_ID ${CMAKE_C_COMPILER} NAME_WE)
get_filename_component(CXX_COMPILER_ID ${CMAKE_CXX_COMPILER} NAME_WE)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(BUILD_TYPE "debug")
else ()
    set(BUILD_TYPE "release")
    list(APPEND WAF_EXTRA_ARGS --enable-poly-opt --enable-lto)
endif ()

if (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(64BIT ON CACHE BOOL "" FORCE)
    list(APPEND WAF_EXTRA_ARGS -8) # only required for x86 when testing this cmakelist under linux
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)

set(WAF_CC "${CMAKE_C_COMPILER} --target=${CMAKE_C_COMPILER_TARGET}")
set(WAF_CXX "${CMAKE_CXX_COMPILER} --target=${CMAKE_CXX_COMPILER_TARGET}")

# not cleanest way to get upper directory
set(ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

file(RELATIVE_PATH SDL_PATH "${ENGINE_SOURCE_DIR}/engine" "${ENGINE_SOURCE_DIR}/3rdparty/SDL")

execute_process(
        COMMAND ${CMAKE_COMMAND} -E env
        CC=${WAF_CC} CXX=${WAF_CXX}
        AR=${CMAKE_AR} STRIP=${CMAKE_STRIP}
        ${PYTHON_EXECUTABLE} waf configure -T ${BUILD_TYPE} -o "${CMAKE_CURRENT_BINARY_DIR}/xash3d-fwgs" ${WAF_EXTRA_ARGS} cmake
        --check-c-compiler=${C_COMPILER_ID} --check-cxx-compiler=${CXX_COMPILER_ID}
        -s "${SDL_PATH}" --skip-sdl2-sanity-check --enable-bundled-deps
        WORKING_DIRECTORY "${ENGINE_SOURCE_DIR}"
)

add_subdirectory("${ENGINE_SOURCE_DIR}/3rdparty/hlsdk-portable" hlsdk-portable)
add_subdirectory("${ENGINE_SOURCE_DIR}/3rdparty/mainui" mainui)
add_subdirectory("${ENGINE_SOURCE_DIR}/" xash3d-fwgs)

# try to build minimal SDL. Enable features as we're gonna use them
foreach (X IN ITEMS SDL_RENDER SDL_POWER SDL_VULKAN SDL_DISKAUDIO SDL_DUMMYAUDIO SDL_DUMMYVIDEO SDL_VULKAN SDL_OFFSCREEN SDL_STATIC)
    set(${X} OFF CACHE BOOL "" FORCE)
endforeach ()
add_subdirectory("${ENGINE_SOURCE_DIR}/3rdparty/SDL" SDL)
