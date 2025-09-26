#pragma once

#include "majin_device.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

struct PipelineConfigInfo {

  PipelineConfigInfo(const PipelineConfigInfo &) = delete;
  PipelineConfigInfo() = default;
  PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
  MajinPipeline &operator=(const MajinPipeline &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

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
