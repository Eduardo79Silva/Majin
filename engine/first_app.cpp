#include "first_app.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>

namespace majin {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{
      _majinDevice, _majinRenderer.getSwapChainRenderPass()};

  while (!_majinWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = _majinRenderer.beginFrame()) {
      _majinRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, _gameObjects);
      _majinRenderer.endSwapChainRenderPass(commandBuffer);
      _majinRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(_majinDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<MajinModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                           {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                           {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto majinModel = std::make_shared<MajinModel>(_majinDevice, vertices);

  auto triangle = MajinGameObject::createGameObject();
  triangle.model = majinModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2D.translation.x = .2f;
  triangle.transform2D.scale = {2.f, .5f};
  triangle.transform2D.rotation = .25f * glm::two_pi<float>();

  _gameObjects.push_back(std::move(triangle));
}

} // namespace majin
