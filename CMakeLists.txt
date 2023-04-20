cmake_minimum_required(VERSION 3.22)

option(MTM_OUTRELDIR "Local installation directory path")
option(MTM_LIBNAME "Output core MTM library's name")
option(MTM_VERSION "Implementation version")

project(mtmobile VERSION ${MTM_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS On)

add_library(mtmobile SHARED)

# https://developer.android.com/ndk/guides/cpp-support#libc


# RTTI is enable by default for ensure a better and safety object's handler
target_compile_options(mtmobile PRIVATE -Wall -Wextra -frtti)
target_link_options(mtmobile PRIVATE $<$<CONFIG:Release>:-s>)

target_sources(mtmobile PRIVATE 
    linux_hierarchy.cpp
    outside.cpp
    patches_level.cpp
    plugin_jni.cpp)

execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(MTM_PROJECTVERSION ${mtmobile_PROJECT_VERSION}.${GIT_HASH})

target_link_libraries(mtmobile m log nativehelper)
set_target_properties(mtmobile PROPERTIES OUTPUT_NAME ${MTM_LIBNAME})
target_include_directories(mtmobile PRIVATE ${mtmobile_SOURCE_DIR})

install(TARGETS mtmobile LIBRARY DESTINATION ${mtmobile_SOURCE_DIR}/${MTM_OUTRELDIR})
