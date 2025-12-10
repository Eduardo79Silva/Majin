#include "gravity_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>

namespace majin {

void GravityPhysicsSystem::update(std::vector<MajinGameObject> &objs, float dt,
                                  unsigned int substeps) {
  const float stepDelta = dt / substeps;

  for (int i = 0; i < substeps; i++) {
    stepSimulation(objs, stepDelta);
  }
}

glm::vec2
GravityPhysicsSystem::computeForce(const MajinGameObject &fromObj,
                                   const MajinGameObject &toObj) const {

  auto offset = fromObj.transform2D.translation - toObj.transform2D.translation;
  float distanceSquared = glm::dot(offset, offset);

  if (glm::abs(distanceSquared) < 1e-10f) {
    return {.0f, .0f};
  }

  float force = strengthGravity * toObj.rigidBody2D.mass *
                fromObj.rigidBody2D.mass / distanceSquared;
  return force * offset / glm::sqrt(distanceSquared);
}

void GravityPhysicsSystem::stepSimulation(
    std::vector<MajinGameObject> &physicsObjs, float dt) {

  for (auto iterA = physicsObjs.begin(); iterA != physicsObjs.end(); ++iterA) {
    auto &objA = *iterA;
    for (auto iterB = iterA; iterB != physicsObjs.end(); ++iterB) {
      if (iterA == iterB)
        continue;
      auto &objB = *iterB;

      auto force = computeForce(objA, objB);
      objA.rigidBody2D.velocity += dt * -force / objA.rigidBody2D.mass;
      objB.rigidBody2D.velocity += dt * force / objB.rigidBody2D.mass;
    }
  }

  for (auto &obj : physicsObjs) {
    obj.transform2D.translation += dt * obj.rigidBody2D.velocity;
  }
}

class Vec2FieldSystem {
public:
  void update(const GravityPhysicsSystem &physicsSystem,
              const std::vector<MajinGameObject> &physicsObjs,
              std::vector<MajinGameObject> &vectorField) {

    for (auto &vf : vectorField) {
      glm::vec2 direction{};
      for (auto &obj : physicsObjs) {
        direction += physicsSystem.computeForce(obj, vf);
      }

      vf.transform2D.scale.x =
          0.005f +
          0.045f *
              glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
      vf.transform2D.rotation = atan2(direction.y, direction.x);
    }
  }
};

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
} // namespace majin
