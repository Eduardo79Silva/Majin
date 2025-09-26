#include "majin_window.hpp"
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace majin {

MajinWindow::MajinWindow(int w, int h, std::string name)
    : _width{w}, _height{h}, _windowName{name} {
  initWindow();
}

MajinWindow::~MajinWindow() {}

void MajinWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  _window =
      glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(_window, this);
  glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
}

void MajinWindow::createWindowSurface(VkInstance instance,
                                      VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, _window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

void MajinWindow::framebufferResizeCallback(GLFWwindow *window, int width,
                                            int height) {
  auto majinWindow =
      reinterpret_cast<MajinWindow *>(glfwGetWindowUserPointer(window));
  majinWindow->_framebufferResized = true;
  majinWindow->_width = width;
  majinWindow->_height = height;
}

} // namespace majin
