cmake_minimum_required(VERSION 3.20)

option(SAMP20_OUTRELDIR "Path to the local installation directory")
option(SAMP20_SHARED_NAME "Name of the output core library")
option(SAMP20_VERSION "Implementation version")

project(samp20 VERSION ${COOP_VERSION} LANGUAGES CXX)
set(RAKNET_ENABLE_STATIC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${PROJECT_SOURCE_DIR})
add_library(coop MODULE)

set(CMAKE_CXX_FLAGS "-DANDROID -Wall -Werror -fno-rtti")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -fno-stack-protector -flto=full -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -Wl,--gc-sections")

add_library(vendor_imgui STATIC
    ${SRC_DIR}/vendor/imgui/imgui_draw.cpp
    ${SRC_DIR}/vendor/imgui/imgui.cpp
    # $<$<CONFIG:DEBUG>:${SRC_DIR}/vendor/imgui/imgui_demo.cpp>
    )
add_subdirectory(src)

target_include_directories(vendor_imgui INTERFACE
    # Does #include <imgui/imgui.h> be available for us
    $<BUILD_INTERFACE:${SRC_DIR}/vendor>
    $<INSTALL_INTERFACE:imgui>)

execute_process(COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(PROJECT_PROJECT_VERSION ${PROJECT_PROJECT_VERSION}.${GIT_HASH})
add_subdirectory("vendor/fmt" EXCLUDE_FROM_ALL)
add_subdirectory("vendor/raknet")

target_link_libraries(samp20 log nativehelper vendor_imgui RakNet fmt::fmt)
set_target_properties(samp20 PROPERTIES OUTPUT_NAME ${SAMP20_SHARED_NAME})
target_include_directories(samp20 PRIVATE ${SRC_DIR})

install(TARGETS samp20 LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/${SAMP20_OUTRELDIR})
