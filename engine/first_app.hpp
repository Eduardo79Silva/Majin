#pragma once

#include "majin_device.hpp"
#include "majin_game_object.hpp"
#include "majin_renderer.hpp"
#include "majin_window.hpp"

#include <vector>

namespace majin {

class FirstApp {
public:
  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

private:
  void loadGameObjects();

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

private:
  MajinWindow _majinWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  MajinDevice _majinDevice{_majinWindow};
  MajinRenderer _majinRenderer{_majinWindow, _majinDevice};
  std::vector<MajinGameObject> _gameObjects;
};

} // namespace majin
