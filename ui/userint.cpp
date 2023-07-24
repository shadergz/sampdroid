
#include <ui/userint.h>
#include <imgui/imgui.h>

#include <log_client.h>

bool ImGui_ImplRenderWare_Init();
void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);

void ImGui_ImplRenderWare_Shutdown();

UiClientUser::UiClientUser()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    [[maybe_unused]] auto& io{ImGui::GetIO()};

    // Dark mode isn't enable by default
    ImGui::StyleColorsDark();

    salog::printFormat(ANDROID_LOG_INFO, "GUI: ImGUI version in use %d: %s", 
        IMGUI_VERSION_NUM, IMGUI_VERSION);

    ImGui_ImplRenderWare_Init();
    
    auto& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;


}

int UiClientUser::renderDrawList()
{
    auto draw{ImGui::GetDrawData()};
    return draw->CmdListsCount;
}

UiClientUser::~UiClientUser()
{
    salog::print(ANDROID_LOG_INFO, "GUI: System is been shutdown now!");
    
    ImGui_ImplRenderWare_Shutdown();
    ImGui::DestroyContext();

}
