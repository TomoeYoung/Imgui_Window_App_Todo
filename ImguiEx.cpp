#include <iostream>
#include "imgui_class.h"
#include "glad/src/glad.h"
#include "glfw/include/GLFW/glfw3.h"

int main() {
    if (!glfwInit()) {
        return 1;
    }

    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    int x, y, width, height;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();

    glfwGetMonitorWorkarea(primary, &x, &y, &width, &height);

    GLFWwindow *window = glfwCreateWindow(width - 1, height - 1, "Locker", nullptr, nullptr);
    if (window == nullptr) {
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw("Unable to context to OpenGl");
    }

    UseImgui myimgui;
    myimgui.Init(window, glsl_version);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        myimgui.HandleClickOutsideImGui(window);
        myimgui.NewFrame();
        myimgui.Update();
        myimgui.Render();
        glfwSwapBuffers(window);
    }
    myimgui.Shutdown();
    return 0;
}
