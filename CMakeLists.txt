cmake_minimum_required(VERSION 3.19)
project(eger)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./include/glfw-3.3.8)
add_subdirectory(./include/glm)
add_subdirectory(./include/assimp)
add_subdirectory(./include/glew-2.1.0)
add_subdirectory(./include/stb_image)
add_subdirectory(./include/imgui)

file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.h")

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    # "libs/imgui/imconfig.h"
    # "libs/imgui/imgui.cpp"
    # "libs/imgui/imgui.h"
    # "libs/imgui/imgui_demo.cpp"
    # "libs/imgui/imgui_draw.cpp"
    # "libs/imgui/imgui_internal.h"
    # "libs/imgui/imgui_tables.cpp"
    # "libs/imgui/imgui_widgets.cpp"
    # "libs/imgui/imstb_rectpack.h"
    # "libs/imgui/imstb_textedit.h"
    # "libs/imgui/imstb_truetype.h"
    # "libs/imgui/imgui_impl_glfw.cpp"
    # "libs/imgui/imgui_impl_glfw.h"
    # "libs/imgui/imgui_impl_glfw.h"
    # "libs/imgui/imgui_impl_glfw.cpp"
    # "libs/imgui/imgui_impl_opengl3.h"
    # "libs/imgui/imgui_impl_opengl3.cpp"
)

target_link_libraries(${PROJECT_NAME}
    glfw
    glm
    assimp
    GLEW
    stb_image
    imgui
    # -lglut
    -lGL
    # -lGLU
)
