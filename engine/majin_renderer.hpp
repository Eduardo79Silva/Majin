#pragma once

#include "majin_device.hpp"
#include "majin_swap_chain.hpp"
#include "majin_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace majin {
class MajinRenderer {
public:
  MajinRenderer(MajinWindow &window, MajinDevice &device);
  ~MajinRenderer();

  MajinRenderer(const MajinRenderer &) = delete;
  MajinRenderer &operator=(const MajinRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return _majinSwapChain->getRenderPass();
  }
  bool isFrameInProgress() const { return _isFrameStarted; }

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(_isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return _commandBuffers[_currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(_isFrameStarted &&
           "Cannot get frame index when frame not in progress");
    return _currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

private:
  MajinWindow &_majinWindow;
  MajinDevice &_majinDevice;
  std::unique_ptr<MajinSwapChain> _majinSwapChain;
  std::vector<VkCommandBuffer> _commandBuffers;

  uint32_t _currentImageIndex{0};
  int _currentFrameIndex{0};
  bool _isFrameStarted{false};
};
} // namespace majin
