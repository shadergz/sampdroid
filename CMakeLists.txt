cmake_minimum_required(VERSION 3.22)

option(SAONLINE_OUTRELDIR "Path to the local installation directory")
option(SAONLINE_SHARED_NAME "Name of the output core library")
option(SAONLINE_VERSION "Implementation version")

project(SAOnline VERSION ${SAONLINE_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${SAOnline_SOURCE_DIR})
add_library(Client MODULE)

target_compile_options(Client PRIVATE -Wall -Werror -fno-rtti
    $<$<CONFIG:Release>:-Ofast;-fno-stack-protector;-DNDEBUG>)
target_link_options(Client PRIVATE $<$<CONFIG:Release>:-s;-Wl,--gc-sections>)

target_sources(Client PRIVATE
    linux_hierarchy.cpp
    log_client.cpp
    patches_level.cpp
    menu_handler.cpp
    plugin_jni.cpp
    nv_threads.cpp
    texture_runtime.cpp
    main_thread.cpp
    jvm_helper.cpp

    render/engine.cpp
    
    game/game_objects.cpp
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

set(SAONLINE_PROJECTVERSION ${SAOnline_PROJECT_VERSION}.${GIT_HASH})
add_subdirectory(vendor/fmt EXCLUDE_FROM_ALL)

target_link_libraries(Client vendor_imgui log nativehelper $<$<CONFIG:Debug>:fmt::fmt>)
set_target_properties(Client PROPERTIES OUTPUT_NAME ${SAONLINE_SHARED_NAME})
target_include_directories(Client PRIVATE ${SRC_DIR})

install(TARGETS Client LIBRARY DESTINATION ${SAOnline_SOURCE_DIR}/${SAONLINE_OUTRELDIR})
