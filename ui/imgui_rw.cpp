#include <log_client.h>
#include <jvm_helper.h>

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
    [[maybe_unused]] auto& globalIo{ImGui::GetIO()};
}

bool ImGui_ImplRenderWare_Init()
{
    auto& io{ImGui::GetIO()};

    const Vec2 dsp{
        saglobal::g_rsGlobal->maximumWidth, 
        saglobal::g_rsGlobal->maximumHeight};
    const ImVec2 displaySz(static_cast<float>(dsp.xWidth), 
        static_cast<float>(dsp.yHeight));
    
    if (displaySz.x > 1)
        io.DisplaySize = displaySz;
    else
        io.DisplaySize = sajvm::getScreenSize();
    // Updates to the screen size will only appear when the next frame buffer is rendered
    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
    
    salog::printFormat(ANDROID_LOG_INFO, "GUI: Client display size: Width = %f, Height = %f", 
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

