#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <string>

namespace majin {

class MajinWindow {
public:
  MajinWindow(int w, int h, std::string name);
  ~MajinWindow();

  MajinWindow(const MajinWindow &) = delete;
  MajinWindow &operator=(const MajinWindow &) = delete;

  bool shouldClose() { return glfwWindowShouldClose(_window); }

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)};
  }
  bool wasWindowResized() { return _framebufferResized; }
  void resetWindowResizedFlag() { _framebufferResized = false; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);
  void initWindow();

private:
  int _width;
  int _height;
  bool _framebufferResized = false;
  std::string _windowName;
  GLFWwindow *_window;
};

} // namespace majin
