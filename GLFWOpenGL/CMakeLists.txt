cmake_minimum_required(VERSION 3.19)
project(OpenGL-GLFW-Engine-Linux)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL-GLFW-Engine-Linux main.cpp stb_image.cpp "Header files/stb_image.h"
        Mesh.cpp "Header files/Mesh.h" Shader.cpp "Header files/Shader.h" Model.cpp "Header files/Model.h"
        "libs/imgui/imconfig.h" "libs/imgui/imgui.cpp" "libs/imgui/imgui.h" "libs/imgui/imgui_demo.cpp"
        "libs/imgui/imgui_draw.cpp" "libs/imgui/imgui_internal.h" "libs/imgui/imgui_tables.cpp"
        "libs/imgui/imgui_widgets.cpp" "libs/imgui/imstb_rectpack.h" "libs/imgui/imstb_textedit.h"
        "libs/imgui/imstb_truetype.h" "libs/imgui/imgui_impl_glfw.cpp" "libs/imgui/imgui_impl_glfw.h"
        "libs/imgui/imgui_impl_glfw.h" "libs/imgui/imgui_impl_glfw.cpp" "libs/imgui/imgui_impl_opengl3.h"
        "libs/imgui/imgui_impl_opengl3.cpp")
target_link_libraries(OpenGL-GLFW-Engine-Linux -lglut -lglfw -lGLEW -lGL -lGLU -lassimp)