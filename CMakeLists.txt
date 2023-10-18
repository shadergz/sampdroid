cmake_minimum_required(VERSION 3.22)

option(COOP_OUTRELDIR "Path to the local installation directory")
option(COOP_SHARED_NAME "Name of the output core library")
option(COOP_VERSION "Implementation version")

project(gtasacoop VERSION ${COOP_VERSION} LANGUAGES CXX)

set(RAKNET_ENABLE_STATIC On)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${gtasacoop_SOURCE_DIR})
add_library(coop MODULE)

set(CMAKE_CXX_FLAGS "-DANDROID -Wall -Werror -fno-rtti")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -fno-stack-protector -flto -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -Wl,--gc-sections")

target_sources(coop PRIVATE
    android/linux_hierarchy.cpp
    client/log_client.cpp
    inj/patches_level.cpp
    game/start/menu_handler.cpp
    android/plugin_jni.cpp
    fixes/nv_threads.cpp
    txt/texture_runtime.cpp
    client/main_thread.cpp
    android/jvm_helper.cpp

    render/engine.cpp
    
    game/objects.cpp
    game/clock.cpp
    game/render.cpp

    ui/user_graphics.cpp
    ui/imgui_rw.cpp)

add_library(vendor_imgui STATIC
    ${SRC_DIR}/vendor/imgui/imgui_draw.cpp
    ${SRC_DIR}/vendor/imgui/imgui.cpp
    # $<$<CONFIG:DEBUG>:${SRC_DIR}/vendor/imgui/imgui_demo.cpp>
    )

target_include_directories(vendor_imgui INTERFACE
    # Does #include <imgui/imgui.h> be available for us
    $<BUILD_INTERFACE:${SRC_DIR}/vendor>
    $<INSTALL_INTERFACE:imgui>)

execute_process(COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(gtasacoop_PROJECT_VERSION ${gtasacoop_PROJECT_VERSION}.${GIT_HASH})
add_subdirectory("vendor/fmt" EXCLUDE_FROM_ALL)
add_subdirectory("vendor/raknet")

target_link_libraries(coop log nativehelper vendor_imgui RakNet fmt::fmt)
set_target_properties(coop PROPERTIES OUTPUT_NAME ${COOP_SHARED_NAME})
target_include_directories(coop PRIVATE ${SRC_DIR})

install(TARGETS coop LIBRARY DESTINATION ${gtasacoop_SOURCE_DIR}/${COOP_OUTRELDIR})
