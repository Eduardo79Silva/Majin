#include "majin_renderer.hpp"

#include <array>
#include <cassert>
#include <stdexcept>

namespace majin {

MajinRenderer::MajinRenderer(MajinWindow &window, MajinDevice &device)
    : _majinWindow{window}, _majinDevice{device} {
  recreateSwapChain();
  createCommandBuffers();
}

MajinRenderer::~MajinRenderer() { freeCommandBuffers(); }

void MajinRenderer::recreateSwapChain() {
  auto extent = _majinWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = _majinWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(_majinDevice.device());

  if (_majinSwapChain == nullptr) {
    _majinSwapChain = std::make_unique<MajinSwapChain>(_majinDevice, extent);
  } else {
    std::shared_ptr<MajinSwapChain> oldSwapChain = std::move(_majinSwapChain);
    _majinSwapChain =
        std::make_unique<MajinSwapChain>(_majinDevice, extent, oldSwapChain);

    if (!oldSwapChain->compareSwapFormats(*_majinSwapChain.get())) {
      throw std::runtime_error(
          "Swap chain image(or depth) format has changed!");
    }
  }
}

void MajinRenderer::createCommandBuffers() {
  _commandBuffers.resize(MajinSwapChain::MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = _majinDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

  if (vkAllocateCommandBuffers(_majinDevice.device(), &allocInfo,
                               _commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void MajinRenderer::freeCommandBuffers() {
  vkFreeCommandBuffers(_majinDevice.device(), _majinDevice.getCommandPool(),
                       static_cast<uint32_t>(_commandBuffers.size()),
                       _commandBuffers.data());
  _commandBuffers.clear();
}

VkCommandBuffer MajinRenderer::beginFrame() {
  assert(!_isFrameStarted && "Can't call beginFrame while already in progress");

  auto result = _majinSwapChain->acquireNextImage(&_currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  _isFrameStarted = true;

  auto commandBuffer = getCurrentCommandBuffer();
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  return commandBuffer;
}

void MajinRenderer::endFrame() {
  assert(_isFrameStarted &&
         "Can't call endFrame while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  auto result = _majinSwapChain->submitCommandBuffers(&commandBuffer,
                                                      &_currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      _majinWindow.wasWindowResized()) {
    _majinWindow.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  _isFrameStarted = false;
  _currentFrameIndex =
      (_currentFrameIndex + 1) % MajinSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void MajinRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(_isFrameStarted &&
         "Can't call beginSwapChainRenderPass if frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Can't begin render pass on command buffer from a different frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = _majinSwapChain->getRenderPass();
  renderPassInfo.framebuffer =
      _majinSwapChain->getFrameBuffer(_currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = _majinSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width =
      static_cast<float>(_majinSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(_majinSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, _majinSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void MajinRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(_isFrameStarted &&
         "Can't call endSwapChainRenderPass if frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Can't end render pass on command buffer from a different frame");
  vkCmdEndRenderPass(commandBuffer);
}

} // namespace majin
