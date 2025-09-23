#pragma once

#include "majin_device.hpp"
#include <cstdint>
#include <cwchar>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

struct PipelineConfigInfo {

  PipelineConfigInfo(const PipelineConfigInfo &) = delete;
  PipelineConfigInfo() = default;
  PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};
class MajinPipeline {
public:
  MajinPipeline(MajinDevice &device, const std::string &vertFilepath,
                const std::string &fragFilepath,
                const PipelineConfigInfo &configInfo);
  ~MajinPipeline();

  MajinPipeline(const MajinPipeline &) = delete;
  void operator=(const MajinPipeline &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo,
                                        uint32_t width, uint32_t height);

private:
  static std::vector<char> readFile(const std::string &filepath);
  void createGraphicsPipeline(const std::string &vertFilepath,
                              const std::string &fragFilepath,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(const std::vector<char> &code,
                          VkShaderModule *shaderModule);

  MajinDevice &_majinDevice;
  VkPipeline _graphicsPipeline;
  VkShaderModule _vertShaderModule;
  VkShaderModule _fragShaderModule;
};
} // namespace majin
