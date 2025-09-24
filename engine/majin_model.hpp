#pragma once

#include "majin_device.hpp"
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace majin {

class MajinModel {

public:
  struct Vertex {
    glm::vec2 position;

    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

  MajinModel(MajinDevice &device, const std::vector<Vertex> &vertices);
  ~MajinModel();

  MajinModel(const MajinModel &) = delete;
  MajinModel &operator=(const MajinModel &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);

private:
  MajinDevice &_majinDevice;
  VkBuffer _vertexBuffer;
  VkDeviceMemory _vertexBufferMemory;
  uint32_t _vertexCount;
};

} // namespace majin
