#include <android/log.h>
#include <imgui/imgui.h>

#include <log_client.h>

#include <render/rwlpcore.h>
#include <render/skeleton.h>

namespace saglobal {
    extern RsGlobalType* g_rsGlobal;
    extern uintptr_t g_gameAddr;
}

struct Vec2 {
    RwInt32 xWidth, yHeight;
};

void ImGui_ImplRenderWare_RenderDrawData([[maybe_unused]] ImDrawData* draw_data)
{

}

bool ImGui_ImplRenderWare_Init()
{
    auto& io{ImGui::GetIO()};

    const Vec2 displaySize{
        saglobal::g_rsGlobal->maximumWidth, 
        saglobal::g_rsGlobal->maximumHeight};
    
    io.DisplaySize = ImVec2(displaySize.xWidth, displaySize.yHeight);
    salog::printFormat(ANDROID_LOG_INFO, "GUI: Client display size: Width = %u, Height = %u\n", 
        io.DisplaySize.x, io.DisplaySize.y);

    return true;
}

void ImGui_ImplRenderWare_CreateDeviceObjects()
{

}

void ImGui_ImplRenderWare_Shutdown()
{
}

void ImGui_ImplRenderWare_NewFrame()
{
    ImGui_ImplRenderWare_CreateDeviceObjects();

    [[maybe_unused]] ImGuiIO& io{ImGui::GetIO()};   
}

