#include "App.h"
#include <iostream>

float abs(const float& x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

App::App(const sf::VideoMode& vm, const std::string& name) : sf::RenderWindow(vm, name) {
    sf::Texture t;
    t.loadFromFile("../textures/hero.png");
    hero_ = new gm::Hero(gm::Model(t), 100, 20);
    bg_ = new gm::smartBg();
    hero_->setPosition((928. - 32) / 2, 600 - (793 - 696));
}

App::~App() {
    loop();
}

bool App::dis() const {
    if (enemy_ == nullptr) return false;
    return abs(hero_->getPosition().x - enemy_->getPosition().x) < 300;
}

void App::loop() {
    enum {
        left,
        right,
        die,
        idle
    } hero_pos = idle;
    while (this->isOpen()) {
        while (hero_->isAlive()) {
            sf::Event ev{};
            while (this->pollEvent(ev)) {
                if (!this->hasFocus()) continue;
                if (ev.type == sf::Event::Closed) {
                    this->close();
                    return;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !dis()) {
                    hero_pos = right;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !dis()) {
                    hero_pos = left;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    hero_->damage(20);
                } else {
                    hero_pos = idle;
                }
            }
            if (hero_pos == idle) {
                hero_->idle(150_ms);
            } else if (hero_pos == left) {
                hero_->left(41_ms);
                bg_->left(41_ms);
            } else if(hero_pos == right) {
                hero_->right(41_ms);
                bg_->right(41_ms);
            }

            this->clear();
            bg_->draw(*this);

            if (dis()) {
                sf::Font font;
                font.loadFromFile("../textures/monogram.ttf");
                sf::Text text(enemy_->question(), font);
                text.setPosition(100, 100);
                text.setColor(sf::Color::White);
                this->draw(text);
            }

            if (enemy_ != nullptr) {
                dynamic_cast<gm::Red*>(enemy_)->idle(82_ms);
                enemy_->draw(*this);
            } else {
                spawnEnemy();
            }
            hero_->draw(*this);
            this->display();
        }
        if (hero_->die(300_ms)) {
            this->close();
            return;
        }
        this->clear();
        bg_->draw(*this);
        if (enemy_ != nullptr) {
            dynamic_cast<gm::Red*>(enemy_)->idle(82_ms);
            enemy_->draw(*this);
        }
        hero_->draw(*this);
        this->display();
    }
}

void App::spawnEnemy() {
    sf::Texture t;
    t.loadFromFile("../textures/dragons/red.png");
    enemy_ = new gm::Red(gm::Model(t), 200, 20);
    enemy_->setPosition(1298, 600 - (793 - 696));
    bg_->fix(*enemy_);
}
