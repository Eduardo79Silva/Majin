#pragma once

#include "majin_device.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

struct PipelineConfigInfo {};
class MajinPipeline {
public:
  MajinPipeline(MajinDevice &device, const std::string &vertFilepath,
                const std::string &fragFilepath,
                const PipelineConfigInfo configInfo);
  ~MajinPipeline() {}

  MajinPipeline(const MajinPipeline &) = delete;
  void operator=(const MajinPipeline &) = delete;

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

private:
  static std::vector<char> readFile(const std::string &filepath);
  void createGraphicsPipeline(const std::string &vertFilepath,
                              const std::string &fragFilepath,
                              const PipelineConfigInfo configInfo);

  void createShaderModule(const std::vector<char> &code,
                          VkShaderModule *shaderModule);

  MajinDevice &_majinDevice;
  VkPipeline _graphicsPipeline;
  VkShaderModule _vertShaderModule;
  VkShaderModule _fragShaderModule;
};
} // namespace majin
