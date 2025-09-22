#pragma once

#include "majin_pipeline.hpp"
#include "majin_window.hpp"

namespace majin {

class FirstApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

private:
  MajinWindow _majinWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  MajinPipeline _majinPipeline{"../shaders/simple_shader.vert.spv",
                               "../shaders/simple_shader.frag.spv"};
};

} // namespace majin
