#include "first_app.hpp"
#include "GLFW/glfw3.h"
#include "majin_model.hpp"
#include "majin_pipeline.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

FirstApp::FirstApp() {
  loadModels();
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}
FirstApp::~FirstApp() {
  vkDestroyPipelineLayout(_majinDevice.device(), _pipelineLayout, nullptr);
}

void FirstApp::run() {
  while (!_majinWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(_majinDevice.device());
}
void FirstApp::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(_majinDevice.device(), &pipelineLayoutInfo,
                             nullptr, &_pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout");
  }
}

void FirstApp::createPipeline() {
  PipelineConfigInfo pipelineConfig{};
  MajinPipeline::defaultPipelineConfigInfo(
      pipelineConfig, _majinSwapChain.width(), _majinSwapChain.height());

  pipelineConfig.renderPass = _majinSwapChain.getRenderPass();
  pipelineConfig.pipelineLayout = _pipelineLayout;

  _majinPipeline = std::make_unique<MajinPipeline>(
      _majinDevice, "../shaders/simple_shader.vert.spv",
      "../shaders/simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::createCommandBuffers() {
  _commandBuffers.resize(_majinSwapChain.imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = _majinDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

  if (vkAllocateCommandBuffers(_majinDevice.device(), &allocInfo,
                               _commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers");
  }

  for (int i = 0; i < _commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _majinSwapChain.getRenderPass();
    renderPassInfo.framebuffer = _majinSwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _majinSwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    _majinPipeline->bind(_commandBuffers[i]);
    _majinModel->bind(_commandBuffers[i]);
    _majinModel->draw(_commandBuffers[i]);

    vkCmdEndRenderPass(_commandBuffers[i]);
    if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer");
    }
  }
}

void FirstApp::drawFrame() {
  uint32_t imageIndex;
  auto result = _majinSwapChain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire next swapchain image");
  }

  result = _majinSwapChain.submitCommandBuffers(&_commandBuffers[imageIndex],
                                                &imageIndex);

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present next swapchain image");
  }
}

void FirstApp::loadModels() {
  std::vector<MajinModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                           {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                           {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  _majinModel = std::make_unique<MajinModel>(_majinDevice, vertices);
}
} // namespace majin
