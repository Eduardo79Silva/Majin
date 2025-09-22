#include "majin_pipeline.hpp"
#include <cstddef>
#include <fstream>
#include <print>
#include <stdexcept>
#include <vector>

namespace majin {

MajinPipeline::MajinPipeline(const std::string &vertFilepath,
                             const std::string &fragFilepath) {
  createGraphicsPipeline(vertFilepath, fragFilepath);
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

void MajinPipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                           const std::string &fragFilepath) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::println("Vertex Shader Code Size: {}", vertCode.size());
  std::println("Fragment Shader Code Size: {}", fragCode.size());
}

} // namespace majin
