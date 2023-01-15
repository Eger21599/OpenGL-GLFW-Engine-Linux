cmake_minimum_required(VERSION 3.12)
project(GLEW)

file(GLOB HEADER_FILES
    "include/GL/*.h"
)

file(GLOB SOURCE_FILES
    "src/*.c"
)

add_library(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include/
)
