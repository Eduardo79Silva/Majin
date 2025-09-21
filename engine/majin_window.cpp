#include "majin_window.hpp"
#include "GLFW/glfw3.h"

namespace majin {

MajinWindow::MajinWindow(int w, int h, std::string name)
    : _width{w}, _height{h}, _windowName{name} {
  initWindow();
}

MajinWindow::~MajinWindow() {}

void MajinWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  _window =
      glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
}

} // namespace majin
