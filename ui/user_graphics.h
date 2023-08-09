#pragma once

#include <cstdio>
#include <vector>

#include <imgui/imgui.h>

class UiClientUser {
public:
    UiClientUser();
    
    ~UiClientUser();

    int renderOnGameScene();
    void renderClientDetails();

    struct SaFont {
        const char* m_fontName;
        char m_fontPathBuffer[0x5f];

        bool m_isLoaded;
        bool m_hasFound;
        bool m_isRequired{true};

        ImFont* m_fontObject;

    };

private:
    std::vector<SaFont*> m_loadedFonts;
    float m_inScreenfontSize = 40.f;
    ImVec2 m_screenScale;

};
