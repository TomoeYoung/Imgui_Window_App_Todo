#include "imgui_class.h"

#include <imgui_internal.h>
#include <TitelbarMy.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glfw/include/GLFW/glfw3.h"

#ifdef _WIN32
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/include/GLFW/glfw3native.h"
#endif

UseImgui::~UseImgui()=default;

void UseImgui::HandleClickOutsideImGui(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsMouseClicked(0))
    {
        if (!io.WantCaptureMouse)
        {
#ifdef _WIN32
            HWND hwnd = glfwGetWin32Window(window);
            if (hwnd)
            {
                ShowWindow(hwnd, SW_MINIMIZE);
            }
#endif
        }
    }
}

void UseImgui::Init(GLFWwindow* window, const char* glsl_version) {
    m_Window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigWindowsResizeFromEdges = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.FrameRounding  = 8.0f;
    style.WindowPadding  = ImVec2(0.0f, 0.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.123f, 0.75f);
    style.Colors[ImGuiCol_ResizeGrip].w        = 0.0f;
    style.Colors[ImGuiCol_ResizeGripHovered].w = 0.0f;
    style.Colors[ImGuiCol_ResizeGripActive].w  = 0.0f;
}

void UseImgui::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UseImgui::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UseImgui::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

TitleBar tb;
enum : uint32_t { BTN_CLOSE = 1, BTN_MIN = 2, BTN_MAX = 3 };

void UseImgui::Update() {

    ImGui::Begin("Locker", nullptr, flags);
    tb.SetOrder({ BTN_CLOSE, BTN_MIN, BTN_MAX });
    tb.AddOrUpdateButton({
        .id = BTN_CLOSE,
        .draw = [](const ImRect& r, float hover_t, float active_t) {},
        .on_click = [this]() {
        }
        }
    );

    tb.AddOrUpdateButton({
        .id = BTN_MIN,
        .draw = [](const ImRect& r, float hover_t, float active_t) {},
        .on_click = [this]() {
            glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
        }
        }
    );


    tb.Draw();

    ImGui::End();
}




