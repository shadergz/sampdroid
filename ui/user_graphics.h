#pragma once

#include <cstdio>
#include <vector>

#include <imgui/imgui.h>

class UiClientUser {
public:
    UiClientUser();
    
    ~UiClientUser();

    int renderOnGameScene();
    void renderVersion();

    struct SaFont {
        SaFont(const char* fontName, bool isRequired = true) : 
            m_fontName(fontName),
            m_isLoaded(false),
            m_hasFound(false),
            m_isRequired(isRequired),
            m_fontObject(nullptr) {}
        
        const char* m_fontName;
        char m_fontPathBuffer[0x5f];

        bool m_isLoaded;
        bool m_hasFound;
        bool m_isRequired;

        ImFont* m_fontObject;

        auto operator*() {
            return m_fontObject;
        }

    };

private:
    std::vector<SaFont*> m_loadedFonts;
    float m_inScreenfontSize = 40.f;
    ImVec2 m_screenScale;

};
