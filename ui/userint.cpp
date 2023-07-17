
#include <ui/userint.h>
#include <imgui/imgui.h>

#include <log_client.h>

UiClientUser::UiClientUser()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    [[maybe_unused]] ImGuiIO& io{ImGui::GetIO()};

    // Dark mode isn't enable by default
    ImGui::StyleColorsDark();
    salog::printFormat(ANDROID_LOG_INFO, "GUI: ImGUI version in use %d: %s", 
        IMGUI_VERSION_NUM, IMGUI_VERSION);

}
UiClientUser::~UiClientUser()
{
    salog::print(ANDROID_LOG_INFO, "GUI system is been shutdown now!");
    ImGui::DestroyContext();

}
