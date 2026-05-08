#pragma once
#include "imgui/backends/imgui_impl_glfw.h"

#ifndef IMGUI_H
#define IMGUI_H

class UseImgui {
public:
    virtual ~UseImgui();
    void Init(GLFWwindow* window, const char* glsl_version);
    virtual void Update();
    void DrawMacLikeTitlebarButtons();
    static void HandleClickOutsideImGui(GLFWwindow* window);
    static void NewFrame();
    static void Render();
    static void Shutdown();
private:
    GLFWwindow* m_Window = nullptr;
};

#endif //IMGUI_H
