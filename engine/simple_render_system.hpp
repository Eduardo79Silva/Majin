#pragma once

#include "majin_device.hpp"
#include "majin_game_object.hpp"
#include "majin_pipeline.hpp"

#include <memory>
#include <vector>

namespace majin {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(MajinDevice &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<MajinGameObject> &gameObjects);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

private:
  MajinDevice &majinDevice;

  std::unique_ptr<MajinPipeline> majinPipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace majin
