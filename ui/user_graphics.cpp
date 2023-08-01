#include <ui/user_graphics.h>
#include <log_client.h>
#include <unistd.h>

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

static UiClientUser::SaFont arialFont {
    .m_fontName = "arial.ttf",
};
static UiClientUser::SaFont sampAux3Font {
    .m_fontName = "sampaux3.ttf",
};
static UiClientUser::SaFont userDefaultFont {
    .m_fontName = "userfont.ttf",
    .m_isRequired = false
};

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
    
    auto& style{ImGui::GetStyle()};
    style.WindowBorderSize = 0.0f;

    // Loading related fonts
    auto gameDataDrive{reinterpret_cast<const char*>(saglobal::g_gameAddr + 0x8b46a8)};

    static SaFont* const SaFontRefs[]{
        &arialFont, 
        &sampAux3Font,
        &userDefaultFont,
        nullptr
    };

    for (auto font : SaFontRefs) {
        if (!font) 
            break;
        
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
        salog::printFormat(salog::Info, "New SA font with name %s successful loaded", font->m_fontName);

    }
}

int UiClientUser::renderDrawCommand()
{
    ImGui_ImplRenderWare_NewFrame();
    ImGui::NewFrame();

    renderClientDetails();

    ImGui::EndFrame();

    // Ensure that ImGUI will render a vertex buffer from its command pipeline list
    ImGui::Render();

    auto draw{ImGui::GetDrawData()};
    ImGui_ImplRenderWare_RenderDrawData(draw);

    return draw->CmdListsCount;
}

void UiClientUser::renderClientDetails()
{
    ImGui::GetOverlayDrawList()->AddText(
        ImVec2(10, 10), ImColor(IM_COL32_BLACK), "SA Mobile");
}

UiClientUser::~UiClientUser()
{
    salog::print(salog::Info, "GUI: System is been shutdown now!");
    
    // auto& io{ImGui::GetIO()};
    // io.Fonts->Clear();

    ImGui_ImplRenderWare_Shutdown();
    ImGui::DestroyContext();

}
