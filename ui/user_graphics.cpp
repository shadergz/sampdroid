#include <array>
#include <unistd.h>

#include <ui/user_graphics.h>
#include <log_client.h>

bool ImGui_ImplRenderWare_Init();

void ImGui_ImplRenderWare_NewFrame();
void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);

void ImGui_ImplRenderWare_Shutdown();

namespace saglobal {
    extern uintptr_t g_gameAddr;
}

static const ImWchar ranges[] {
    0x0020, 0x0080,
    0x00A0, 0x00C0,
    0x0400, 0x0460,
    0x0490, 0x04A0,
    0x2010, 0x2040,
    0x20A0, 0x20B0,
    0x2110, 0x2130,
    0
};

static UiClientUser::SaFont arialFont("arial.ttf");
static UiClientUser::SaFont sampAux3Font("sampaux3.ttf");
static UiClientUser::SaFont userDefaultFont("userfont.ttf", false);

UiClientUser::UiClientUser()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ImGui::GetIO()};

    // Dark mode isn't enable by default
    ImGui::StyleColorsDark();

    salog::printFormat(salog::Info, "GUI: ImGUI version in use %d: %s", 
        IMGUI_VERSION_NUM, IMGUI_VERSION);

    ImGui_ImplRenderWare_Init();
    m_screenScale.x = io.DisplaySize.x * (1 / io.DisplaySize.x);
    m_screenScale.y = io.DisplaySize.y * (1 / io.DisplaySize.y);
    
    auto& style{ImGui::GetStyle()};
    style.ScrollbarSize = m_screenScale.y * 55.f;
    style.WindowBorderSize = 0.0f;

    // Loading related fonts
    auto gameDataDrive{reinterpret_cast<const char*>(saglobal::g_gameAddr + 0x8b46a8)};

    std::array<SaFont*, 1> saFontPtrs {
        // &sampAux3Font, &userDefaultFont, 
        &arialFont
    };

    for (auto font : saFontPtrs) {   
        std::snprintf(font->m_fontPathBuffer, std::size(font->m_fontPathBuffer),
            "%sfonts/%s", gameDataDrive, font->m_fontName);

        salog::printFormat(salog::Debug, "Attempt to load font %s from %s", 
            font->m_fontName, font->m_fontPathBuffer);

        bool isFound{false};
        if (font->m_isRequired && !(isFound = access(font->m_fontPathBuffer, R_OK | F_OK) == 0)) {
            salog::printFormat(salog::Error, "Couldn't load font with name %s", font->m_fontName);
            std::terminate();
        }

        if (!isFound)
            continue;
        
        font->m_fontObject = io.Fonts->AddFontFromFileTTF(font->m_fontPathBuffer, 
            m_inScreenfontSize, nullptr, ranges);
        
        m_loadedFonts.push_back(font);
        salog::printFormat(salog::Info, "GUI: new SA font with name %s has successful loaded", font->m_fontName);

    }

}

int UiClientUser::renderOnGameScene()
{
    ImGui_ImplRenderWare_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Demo window");
    ImGui::Button("Hello!");
    ImGui::End();

    renderVersion();

#ifndef NDEBUG
    // ImGui::ShowDemoWindow();
#endif
    ImGui::EndFrame();

    // Ensure that ImGUI will render a vertex buffer from its command pipeline list
    ImGui::Render();

    // Render everything to the screen using our dynamic RW vertex buffer
    auto draw{ImGui::GetDrawData()};
    ImGui_ImplRenderWare_RenderDrawData(draw);

    return draw->CmdListsCount;
}

void UiClientUser::renderVersion()
{
    ImGui::GetOverlayDrawList()->AddText(
        ImVec2(m_screenScale.x * 20, m_screenScale.y * 1000),
        ImColor(IM_COL32_BLACK), "SA Mobile v0.103");
}

UiClientUser::~UiClientUser()
{
    salog::print(salog::Info, "GUI: system is been shutdown now!");
    
    auto& io{ImGui::GetIO()};
    io.Fonts->Clear();

    ImGui_ImplRenderWare_Shutdown();
    ImGui::DestroyContext();

}
