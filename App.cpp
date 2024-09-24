#include "App.h"
#include <iostream>


float abs(const float& x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

Text::Text() {
    font_.loadFromFile("../textures/monogram.ttf");
}

Text::Text(const std::string& text, const std::string& filename) {
    font_.loadFromFile(filename);
    this->setString(text);
    this->setColor(sf::Color::Black);
    this->setFont(font_);
}

Dialog::Dialog() {
    label_.setPosition(this->getPosition() + sf::Vector2f{15, 15});
    input_.setPosition(this->getPosition() + sf::Vector2f{39, 72});
    t_.loadFromFile("../textures/dialog.png");
    this->setTexture(t_);
}

std::string Dialog::get_ans() const {
    return input_.getString();
}

void Dialog::draw(sf::RenderWindow& window) const {
    window.draw(*this);
    window.draw(label_);
    window.draw(input_);
}

void Dialog::input(const char& x) {
    if (x < '0' || '9' < x) return;
    input_.setString(input_.getString() + x);
}

void Dialog::setLabel(const std::string& s) {
    label_.setString(s);
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
        idle,
        answered
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
                    ;
                } else {
                    hero_pos = idle;
                }
            }

            if (dis() && q_ == nullptr) {
                question();
            }

            if (hero_pos == idle) {
                hero_->idle(150_ms);
            } else if (hero_pos == left) {
                hero_->left(41_ms);
                bg_->left(41_ms);
            } else if (hero_pos == right) {
                hero_->right(41_ms);
                bg_->right(41_ms);
            }

            this->clear();
            bg_->draw(*this);

            if (enemy_ != nullptr) {
                if (dynamic_cast<gm::Red*>(enemy_) != nullptr) {
                    dynamic_cast<gm::Red*>(enemy_)->idle(150_ms);
                } else if (dynamic_cast<gm::Green*>(enemy_) != nullptr) {
                    dynamic_cast<gm::Green*>(enemy_)->idle(150_ms);
                } else if (dynamic_cast<gm::Black*>(enemy_) != nullptr) {
                    dynamic_cast<gm::Black*>(enemy_)->idle(150_ms);
                } else {
                    dynamic_cast<gm::Troll*>(enemy_)->idle(150_ms);
                }
                enemy_->draw(*this);
            } else {
                spawnEnemy();
            }

            if (q_ != nullptr) {
                this->draw(*q_);
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
            if (dynamic_cast<gm::Red*>(enemy_) != nullptr) {
                dynamic_cast<gm::Red*>(enemy_)->idle(150_ms);
            } else if (dynamic_cast<gm::Green*>(enemy_) != nullptr) {
                dynamic_cast<gm::Green*>(enemy_)->idle(150_ms);
            } else if (dynamic_cast<gm::Black*>(enemy_) != nullptr) {
                dynamic_cast<gm::Black*>(enemy_)->idle(150_ms);
            } else {
                dynamic_cast<gm::Troll*>(enemy_)->idle(150_ms);
            }
            enemy_->draw(*this);
        }
        hero_->draw(*this);
        this->display();
    }
}

void App::spawnEnemy() {
    sf::Texture t;
    t.loadFromFile("../textures/dragons/red.png");

    auto red = new gm::Red(gm::Model(t), 100, 20);
    t.loadFromFile("../textures/dragons/green_ani.png");
    auto green = new gm::Green(gm::Model(t), 100, 20);
    t.loadFromFile("../textures/dragons/black_ani.png");
    auto black = new gm::Black(gm::Model(t), 250, 40);

    t.loadFromFile("../textures/troll.png");
    auto troll = new gm::Troll(gm::Model(t), 50, 10);

    if (const auto num = gm::generator() % 100; num >= 90) {
        enemy_ = black;
        delete red;
        delete green;
        delete troll;
    } else if (num >= 60) {
        enemy_ = troll;
        delete red;
        delete green;
        delete black;
    } else if (num >= 30) {
        enemy_ = red;
        delete troll;
        delete green;
        delete black;
    } else {
        enemy_ = green;
        delete troll;
        delete red;
        delete black;
    }

    enemy_->setPosition(1298, 600 - (793 - 696));
    bg_->fix(*enemy_);
}

void App::question() {
    q_ = new Text(enemy_->question());
}
