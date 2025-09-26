#include "first_app.hpp"
#include "GLFW/glfw3.h"
#include "majin_model.hpp"
#include "majin_pipeline.hpp"
#include "majin_swap_chain.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace majin {

struct SimplePushConstantData {
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

FirstApp::FirstApp() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
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

  VkPushConstantRange pushConstantRange{
      .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      .offset = 0,
      .size = 128};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

  if (vkCreatePipelineLayout(_majinDevice.device(), &pipelineLayoutInfo,
                             nullptr, &_pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout");
  }
}

void FirstApp::createPipeline() {
  assert(_majinSwapChain != nullptr &&
         "Cannot create pipeline before swap chain");
  assert(_pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  MajinPipeline::defaultPipelineConfigInfo(pipelineConfig);

  pipelineConfig.renderPass = _majinSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = _pipelineLayout;

  _majinPipeline = std::make_unique<MajinPipeline>(
      _majinDevice, "../shaders/simple_shader.vert.spv",
      "../shaders/simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::recreateSwapChain() {
  auto extent = _majinWindow.getExtent();

  while (extent.width == 0 || extent.height == 0) {
    extent = _majinWindow.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(_majinDevice.device());

  if (_majinSwapChain == nullptr) {
    _majinSwapChain = std::make_unique<MajinSwapChain>(_majinDevice, extent);

  } else {
    _majinSwapChain = std::make_unique<MajinSwapChain>(
        _majinDevice, extent, std::move(_majinSwapChain));

    if (_majinSwapChain->imageCount() != _commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  // TODO: if render pass is compatible there is no need to recreate the
  // pipeline
  createPipeline();
}

void FirstApp::createCommandBuffers() {
  _commandBuffers.resize(_majinSwapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = _majinDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

  if (vkAllocateCommandBuffers(_majinDevice.device(), &allocInfo,
                               _commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers");
  }
}

void FirstApp::freeCommandBuffers() {
  vkFreeCommandBuffers(_majinDevice.device(), _majinDevice.getCommandPool(),
                       static_cast<uint32_t>(_commandBuffers.size()),
                       _commandBuffers.data());

  _commandBuffers.clear();
}

void FirstApp::recordCommandBuffer(int imageIndex) {
  static int frame = 0;
  frame = (frame + 1) % 1000;
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = _majinSwapChain->getRenderPass();
  renderPassInfo.framebuffer = _majinSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = _majinSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo,
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
  vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);

  _majinPipeline->bind(_commandBuffers[imageIndex]);
  _majinModel->bind(_commandBuffers[imageIndex]);

  for (int j = 0; j < 4; j++) {
    SimplePushConstantData push{
        .offset = {-0.5f + frame * 0.02f, -0.4f + j * 0.25f},
        .color = {0.0f, 0.0f, 0.2f + 0.2f * j}};

    vkCmdPushConstants(_commandBuffers[imageIndex], _pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);
    _majinModel->draw(_commandBuffers[imageIndex]);
  }

  vkCmdEndRenderPass(_commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer");
  }
}

void FirstApp::drawFrame() {
  uint32_t imageIndex;
  auto result = _majinSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire next swapchain image");
  }

  recordCommandBuffer(imageIndex);
  result = _majinSwapChain->submitCommandBuffers(&_commandBuffers[imageIndex],
                                                 &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      _majinWindow.wasWindowResized()) {
    _majinWindow.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }

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
