#include "majin_game_object.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace majin {

class GravityPhysicsSystem {

public:
  explicit GravityPhysicsSystem(float strength) : strengthGravity{strength} {}

  const float strengthGravity;

  void update(std::vector<MajinGameObject> &objs, float dt,
              unsigned int substeps = 1);

  glm::vec2 computeForce(const MajinGameObject &fromObj,
                         const MajinGameObject &toObj) const;

private:
  void stepSimulation(std::vector<MajinGameObject> &physicsObjs, float dt);
};

class Vec2FieldSystem {
public:
  void update(const GravityPhysicsSystem &physicsSystem,
              const std::vector<MajinGameObject> &physicsObjs,
              std::vector<MajinGameObject> &vectorField);
};
} // namespace majin
