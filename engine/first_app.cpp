#include "first_app.hpp"
#include "GLFW/glfw3.h"

namespace majin {

void FirstApp::run() {
  while (!_majinWindow.shouldClose()) {
    glfwPollEvents();
  }
}

} // namespace majin
