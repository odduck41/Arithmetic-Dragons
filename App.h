#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Text final : public sf::Text {
  public:
    Text();
    explicit Text(const std::string&, const std::string& = "../textures/monogram.ttf");
  private:
    sf::Font font_;
};


class Dialog final : public sf::Sprite {
  public:
    Dialog();
    void input(const char&);
    void setLabel(const std::string&);
    std::string get_ans() const;
    void draw(sf::RenderWindow&);
    void last();
  private:
    Text input_{};
    Text label_{};
    sf::Texture t_{};
};

class App final : sf::RenderWindow {
  public:
    App(const sf::VideoMode&, const std::string&);
    ~App() override;
  private:
    bool dis() const;
    void loop();
    void spawnEnemy();
    void question();
    gm::smartBg* bg_{};
    gm::Hero* hero_;
    gm::Enemy* enemy_{};
    Dialog* q_{};
};

