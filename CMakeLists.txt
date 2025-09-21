cmake_minimum_required(VERSION 3.20)
project(ToS3D)

set(CMAKE_CXX_STANDARD 23)

# Vulkan
find_package(Vulkan REQUIRED)

# GLFW
add_subdirectory(third_party/glfw)

# Engine library
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(engine ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC engine)
target_link_libraries(engine Vulkan::Vulkan glfw)

# App executable
add_executable(app
    app/main.cpp
)
target_include_directories(app PUBLIC app)
target_link_libraries(app engine)

