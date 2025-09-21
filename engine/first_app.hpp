#pragma once

#include "majin_window.hpp"

namespace majin {

class FirstApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

private:
  MajinWindow _majinWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
};

} // namespace majin
