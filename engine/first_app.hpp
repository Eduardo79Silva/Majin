#pragma once

#include "majin_device.hpp"
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
  MajinDevice _majinDevice{_majinWindow};
  MajinPipeline _majinPipeline{
      _majinDevice, "../shaders/simple_shader.vert.spv",
      "../shaders/simple_shader.frag.spv",
      MajinPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};

} // namespace majin
