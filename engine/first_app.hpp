#pragma once

#include "majin_device.hpp"
#include "majin_model.hpp"
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
  void freeCommandBuffers();
  void drawFrame();
  void loadModels();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

private:
  MajinWindow _majinWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  MajinDevice _majinDevice{_majinWindow};
  VkPipelineLayout _pipelineLayout;
  std::unique_ptr<MajinSwapChain> _majinSwapChain;
  std::unique_ptr<MajinPipeline> _majinPipeline;
  std::unique_ptr<MajinModel> _majinModel;
  std::vector<VkCommandBuffer> _commandBuffers;
};

} // namespace majin
