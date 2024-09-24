#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class App final : sf::RenderWindow {
  public:
    App(const sf::VideoMode&, const std::string&);
    ~App() override;
  private:
    bool dis() const;
    void loop();
    void spawnEnemy();
    gm::smartBg* bg_{};
    gm::Hero* hero_;
    gm::Enemy* enemy_{};
};
