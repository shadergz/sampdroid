#include <jni.h>
#include <span>
#include <imgui/imgui.h>

uint getPackageIdentifier(std::span<char> packageNameId);
ImVec2 getScreenSize();
