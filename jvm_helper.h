#include <jni.h>

#include <span>

#include <imgui/imgui.h>

namespace sajvm {
    uint getPackageIdentifier(std::span<char> packageNameId);
    ImVec2 getScreenSize();

}
