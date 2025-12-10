cmake_minimum_required(VERSION 3.20)
project(Majin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(
        copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    )
endif()

find_package(Vulkan REQUIRED)
add_subdirectory(third_party/glfw)

file(GLOB_RECURSE SHADER_SOURCES CONFIGURE_DEPENDS
    "shaders/*.vert"
    "shaders/*.frag"
)

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SHADER_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER_NAME}.spv")

    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_OUTPUT}
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER_NAME}"
    )

    list(APPEND SHADER_BINARIES ${SHADER_OUTPUT})
endforeach()

add_custom_target(compile_shaders ALL DEPENDS ${SHADER_BINARIES})

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(engine ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC engine)
target_link_libraries(engine Vulkan::Vulkan glfw)

add_executable(app app/main.cpp)
target_include_directories(app PUBLIC app)
target_link_libraries(app engine)

add_dependencies(app compile_shaders)
