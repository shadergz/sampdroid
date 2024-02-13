target_sources(samp20 PRIVATE
    core/log_client.cpp
    core/main_thread.cpp
    
    game/clock.cpp
    inj/jvm_helper.cpp
    game/linux_hierarchy.cpp
    game/menu_handler.cpp
    game/objects.cpp
    inj/plugin_jni.cpp
    game/render.cpp
    
    inj/jvm_helper.cpp
    inj/patches_level.cpp
    inj/plugin_jni.cpp
    
    ui/imgui_rw.cpp
    ui/user_graphics.cpp

    engine.cpp
    nv_threads.cpp
    texture_runtime.cpp
)