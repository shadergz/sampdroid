cmake_minimum_required(VERSION 3.22)

option(SAMOBILE_OUTRELDIR "Local installation directory path")
option(SAMOBILE_SHARED_NAME "Output core library's name")
option(SAMOBILE_VERSION "Implementation version")

project(SAMobile VERSION ${SAMOBILE_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS On)
add_library(saclient MODULE)

# https://developer.android.com/ndk/guides/cpp-support#libc
# RTTI is enable by default for ensure a better and safety objects handle
target_compile_options(saclient PRIVATE -Wall -Wextra -frtti $<$<CONFIG:Release>:-Ofast;-DNDEBUG>)
target_link_options(saclient PRIVATE $<$<CONFIG:Release>:-s;-Wl,--gc-sections>)
target_sources(saclient PRIVATE
    linux_hierarchy.cpp
    log_client.cpp
    patches_level.cpp
    menu_handler.cpp
    plugin_jni.cpp
    nv_threads.cpp
    texture_runtime.cpp

    render/engine.cpp
    game/sacore.cpp
    )

execute_process(COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(SAMOBILE_PROJECTVERSION ${saclient_PROJECT_VERSION}.${GIT_HASH})
add_subdirectory(vendor/fmt EXCLUDE_FROM_ALL)

target_link_libraries(saclient log nativehelper $<$<CONFIG:Debug>:fmt::fmt>)
set_target_properties(saclient PROPERTIES OUTPUT_NAME ${SAMOBILE_SHARED_NAME})
target_include_directories(saclient PRIVATE ${SAMobile_SOURCE_DIR})

install(TARGETS saclient LIBRARY DESTINATION ${SAMobile_SOURCE_DIR}/${SAMOBILE_OUTRELDIR})
