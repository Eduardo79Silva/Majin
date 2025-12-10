#include "first_app.hpp"

#include "gravity_system.hpp"
#include "majin_model.hpp"
#include "simple_render_system.hpp"
#include <memory>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>

namespace majin {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

std::unique_ptr<MajinModel> createSquareModel(MajinDevice &device,
                                              glm::vec2 offset) {
  std::vector<MajinModel::Vertex> vertices = {
      {{-0.5f, -0.5f}}, {{0.5f, 0.5f}},  {{-0.5f, 0.5f}},
      {{-0.5f, -0.5f}}, {{0.5f, -0.5f}}, {{0.5f, 0.5f}}, //
  };
  for (auto &v : vertices) {
    v.position += offset;
  }
  return std::make_unique<MajinModel>(device, vertices);
}

std::unique_ptr<MajinModel> createCircleModel(MajinDevice &device,
                                              unsigned int numSides) {
  std::vector<MajinModel::Vertex> uniqueVertices{};

  for (int i = 0; i < numSides; i++) {
    float angle = i * glm::two_pi<float>() / numSides;
    uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
  }

  uniqueVertices.push_back({});
  std::vector<MajinModel::Vertex> vertices{};

  for (int i = 0; i < numSides; i++) {
    vertices.push_back(uniqueVertices[i]);
    vertices.push_back(uniqueVertices[(i + 1) % numSides]);
    vertices.push_back(uniqueVertices[numSides]);
  }

  return std::make_unique<MajinModel>(device, vertices);
}

void FirstApp::run() {
  std::shared_ptr<MajinModel> squareModel =
      createSquareModel(_majinDevice, {.5f, .0f});
  std::shared_ptr<MajinModel> circleModel = createCircleModel(_majinDevice, 64);

  std::vector<MajinGameObject> physicsObjects{};

  auto red = MajinGameObject::createGameObject();
  red.transform2D.scale = glm::vec2{.05f};
  red.transform2D.translation = {.5f, .5f};
  red.color = {1.f, 0.f, 0.f};
  red.rigidBody2D.velocity = {-.5f, .0f};
  red.model = circleModel;
  physicsObjects.push_back(std::move(red));

  auto blue = MajinGameObject::createGameObject();
  blue.transform2D.scale = glm::vec2{.05f};
  blue.transform2D.translation = {-.45f, -.25f};
  blue.color = {0.f, 0.f, 1.f};
  blue.rigidBody2D.velocity = {.5f, .0f};
  blue.model = circleModel;
  physicsObjects.push_back(std::move(blue));

  std::vector<MajinGameObject> vectorField{};
  int gridCount = 40;

  for (int i = 0; i < gridCount; i++) {
    for (int j = 0; j < gridCount; j++) {
      auto vf = MajinGameObject::createGameObject();
      vf.transform2D.scale = glm::vec2(0.005f);
      vf.transform2D.translation = {-1.0f + (i + 0.5f) * 2.0f / gridCount,
                                    -1.0f + (j + 0.5f) * 2.0f / gridCount};
      vf.color = glm::vec3(1.0f);
      vf.model = squareModel;
      vectorField.push_back(std::move(vf));
    }
  }

  GravityPhysicsSystem gravitySystem{0.81f};
  Vec2FieldSystem vecFieldSystem{};

  SimpleRenderSystem simpleRenderSystem{
      _majinDevice, _majinRenderer.getSwapChainRenderPass()};

  while (!_majinWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = _majinRenderer.beginFrame()) {
      gravitySystem.update(physicsObjects, 1.f / 60, 5);
      vecFieldSystem.update(gravitySystem, physicsObjects, vectorField);

      _majinRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, physicsObjects);
      simpleRenderSystem.renderGameObjects(commandBuffer, vectorField);
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
