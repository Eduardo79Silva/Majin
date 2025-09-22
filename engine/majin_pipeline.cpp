#include "majin_pipeline.hpp"
#include "majin_device.hpp"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <print>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

MajinPipeline::MajinPipeline(MajinDevice &device,
                             const std::string &vertFilepath,
                             const std::string &fragFilepath,
                             const PipelineConfigInfo configInfo)
    : _majinDevice{device} {
  createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

PipelineConfigInfo MajinPipeline::defaultPipelineConfigInfo(uint32_t width,
                                                            uint32_t height) {
  PipelineConfigInfo configInfo{};

  return configInfo;
}

std::vector<char> MajinPipeline::readFile(const std::string &filepath) {
  std::ifstream file{filepath, std::ifstream::ate | std::ifstream::binary};

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file");
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

void MajinPipeline::createGraphicsPipeline(
    const std::string &vertFilepath, const std::string &fragFilepath,
    const PipelineConfigInfo configInfo) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::println("Vertex Shader Code Size: {}", vertCode.size());
  std::println("Fragment Shader Code Size: {}", fragCode.size());
}

void MajinPipeline::createShaderModule(const std::vector<char> &code,
                                       VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(_majinDevice.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module");
  }
}

} // namespace majin
