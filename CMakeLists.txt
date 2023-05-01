cmake_minimum_required(VERSION 3.22)

option(MTM_OUTRELDIR "Local installation directory path")
option(MTM_LIBNAME "Output core MTM library's name")
option(MTM_VERSION "Implementation version")

project(mt4mobile VERSION ${MTM_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS On)

add_library(mt4mobile SHARED)

# https://developer.android.com/ndk/guides/cpp-support#libc


# RTTI is enable by default for ensure a better and safety object's handler
target_compile_options(mt4mobile PRIVATE -Wall -Wextra -frtti
    $<$<CONFIG:Release>:-ffunction-sections;-fdata-sections>)
target_link_options(mt4mobile PRIVATE $<$<CONFIG:Release>:-s;-Wl,--gc-sections>)

target_sources(mt4mobile PRIVATE 
    linux_hierarchy.cpp
    outside.cpp
    patches_level.cpp
    menu_handler.cpp
    plugin_jni.cpp
    nv_threads.cpp
    texture_runtime.cpp

    rwrefs/engine.cpp
    )

execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(MTM_PROJECTVERSION ${mt4mobile_PROJECT_VERSION}.${GIT_HASH})

add_subdirectory(vendor/fmt EXCLUDE_FROM_ALL)

target_link_libraries(mt4mobile m fmt::fmt log nativehelper)
set_target_properties(mt4mobile PROPERTIES OUTPUT_NAME ${MTM_LIBNAME})
target_include_directories(mt4mobile PRIVATE ${mt4mobile_SOURCE_DIR})

install(TARGETS mt4mobile LIBRARY DESTINATION ${mt4mobile_SOURCE_DIR}/${MTM_OUTRELDIR})
