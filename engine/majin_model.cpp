#include "majin_model.hpp"
#include <cstddef>
#include <cstring>
#include <urlmon.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace majin {

MajinModel::MajinModel(MajinDevice &device, const std::vector<Vertex> &vertices)
    : _majinDevice{device} {
  createVertexBuffers(vertices);
}

MajinModel::~MajinModel() {
  vkDestroyBuffer(_majinDevice.device(), _vertexBuffer, nullptr);
  vkFreeMemory(_majinDevice.device(), _vertexBufferMemory, nullptr);
}

void MajinModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
  _vertexCount = static_cast<uint32_t>(vertices.size());

  assert(_vertexCount >= 3 && "Vertex count must be at least 3");

  VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
  _majinDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            _vertexBuffer, _vertexBufferMemory);

  void *data;
  vkMapMemory(_majinDevice.device(), _vertexBufferMemory, 0, bufferSize, 0,
              &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(_majinDevice.device(), _vertexBufferMemory);
}

void MajinModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {_vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void MajinModel::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription>
MajinModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
MajinModel::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);

  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
  return attributeDescriptions;
}

} // namespace majin
