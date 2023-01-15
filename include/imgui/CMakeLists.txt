cmake_minimum_required(VERSION 3.0)
project(imgui)

find_package(glfw)

file(GLOB SOURCE_FILES "imgui/*.cpp")
file(GLOB HEADER_FILES "imgui/*.h")

add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
)

