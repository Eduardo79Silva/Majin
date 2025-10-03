#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#pragma one

#include "majin_model.hpp"

namespace majin {

struct Transform2DComponent {
  glm::vec2 translation{};
  glm::vec2 scale{1.f, 1.f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, 0}, {0, scale.y}};
    return rotMatrix * scaleMat;
  };
};

class MajinGameObject {
public:
  using id_t = unsigned int;

  static MajinGameObject createGameObject() {
    static id_t currentId = 0;
    return MajinGameObject{currentId++};
  }

  MajinGameObject(const MajinGameObject &) = delete;
  MajinGameObject &operator=(const MajinGameObject &) = delete;
  MajinGameObject(MajinGameObject &&) = default;
  MajinGameObject &operator=(MajinGameObject &&) = default;

  const id_t getId() { return m_id; }

private:
  MajinGameObject(id_t objId) : m_id{objId} {}

public:
  std::shared_ptr<MajinModel> model{};
  glm::vec3 color{};
  Transform2DComponent transform2D{};

private:
  id_t m_id;
};

} // namespace majin
