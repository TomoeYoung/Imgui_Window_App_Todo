cmake_minimum_required(VERSION 3.28)
project(timp_lab_1)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(glfw)


add_executable(timp_lab_1

        imguiEx.cpp
        imgui_class.cpp
        widgets.cpp

        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        glad/src/glad.c

         imgui/backends/imgui_impl_glfw.cpp
         imgui/backends/imgui_impl_opengl3.cpp
        TitelbarMy.cpp
        draw_and_animation_functions.cpp
        draw_and_animation_functions.h
)


target_include_directories(timp_lab_1 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/src
        ${glfw_SOURCE_DIR}/include
)


target_link_libraries(timp_lab_1 PRIVATE
        glfw
        opengl32
        gdi32
)