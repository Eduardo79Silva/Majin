#pragma once

#include "majin_device.hpp"
#include "majin_pipeline.hpp"
#include "majin_swap_chain.hpp"
#include "majin_window.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

class FirstApp {
public:
  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

private:
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

private:
  MajinWindow _majinWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  MajinDevice _majinDevice{_majinWindow};
  MajinSwapChain _majinSwapChain{_majinDevice, _majinWindow.getExtent()};
  std::unique_ptr<MajinPipeline> _majinPipeline;
  VkPipelineLayout _pipelineLayout;
  std::vector<VkCommandBuffer> _commandBuffers;
};

} // namespace majin
