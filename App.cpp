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
    this->setFont(font_);
}

Text::Text(const std::string& text, const std::string& filename) {
    font_.loadFromFile(filename);
    this->setString(text);
    this->setFont(font_);
}

Dialog::Dialog() {
    label_.setFillColor(sf::Color::Black);
    input_.setFillColor(sf::Color::Black);
    t_.loadFromFile("../textures/dialog.png");
    this->setTexture(t_);
}

std::string Dialog::get_ans() const {
    return input_.getString();
}

void Dialog::draw(sf::RenderWindow& window) {
    label_.setPosition(this->getPosition() + sf::Vector2f{30, 30});
    input_.setPosition(this->getPosition() + sf::Vector2f{80, 144});
    window.draw(*this);
    window.draw(label_);
    window.draw(input_);
}

void Dialog::last() {
    input_.setString(input_.getString().substring(0, input_.getString().getSize() - 1));
}

void Dialog::input(const char& x) {
    if (x != '-' && (x < '0' || '9' < x)) return;
    if (input_.getString().getSize() == 7) return;
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
    return enemy_->getPosition().x - hero_->getPosition().x <= 300;
}

void App::loop() {
    enum {
        left,
        right,
        idle,
        attack
    } hero_pos = idle;

    enum {
        espeak,
        eattack,
        eidle
    } enemy_pos = eidle;
    bool hattack = true, e_attack = true;
    Text hero_hp, enemy_hp;
    hero_hp.setFillColor(sf::Color::White);
    enemy_hp.setFillColor(sf::Color::White);
    hero_hp.setPosition(10, 10);
    enemy_hp.setPosition(870, 10);
    while (this->isOpen()) {
        while (hero_->isAlive()) {
            sf::Event ev{};
            while (this->pollEvent(ev)) {
                // if (!this->hasFocus()) continue;
                if (ev.type == sf::Event::Closed) {
                    this->close();
                    return;
                }
                if (!e_attack || !hattack) break;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !dis()) {
                    hero_pos = right;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !dis()) {
                    hero_pos = left;
                } else if (q_ != nullptr && ev.type == sf::Event::TextEntered) {
                    hero_pos = idle;
                    q_->input(static_cast<char>(ev.text.unicode));
                } else if (q_ != nullptr && ev.type == sf::Event::KeyPressed &&
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                    hero_pos = idle;
                    q_->last();
                } else if (q_ != nullptr && ev.type == sf::Event::KeyPressed &&
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    if (enemy_->answer(std::stoll(q_->get_ans()))) {
                        hero_pos = attack;
                        hattack = false;
                        hero_->give_damage(*enemy_);
                    } else {
                        enemy_pos = eattack;
                        e_attack = false;
                        enemy_->give_damage(*hero_);
                    }
                    delete q_;
                    q_ = nullptr;
                }
            }
            if (hattack && enemy_ != nullptr && !enemy_->isAlive()) {
                delete enemy_;
                enemy_ = nullptr;
                enemy_pos = eidle;
                delete q_;
                q_ = nullptr;
                hero_pos = idle;
            }
            if (hattack && e_attack && dis() && q_ == nullptr) {
                question();
                if (dynamic_cast<gm::Troll*>(enemy_) != nullptr) {
                    enemy_pos = espeak;
                }
            }

            if (hero_pos == idle && hattack) {
                hero_->idle(150_ms);
            } else if (hero_pos == left && hattack) {
                hero_->left(41_ms);
                bg_->left(41_ms);
            } else if (hero_pos == right && hattack) {
                hero_->right(41_ms);
                bg_->right(41_ms);
            } else {
                hattack = hero_->attack(150_ms);
            }
            if (hattack) {
                hero_pos = idle;
            }

            hero_hp.setString(std::to_string(hero_->HP()));
            this->clear();
            bg_->draw(*this);

            if (enemy_ != nullptr) {
                enemy_hp.setString(std::to_string(enemy_->HP()));
                if (dynamic_cast<gm::Red*>(enemy_) != nullptr) {
                    if (enemy_pos == eidle) {
                        dynamic_cast<gm::Red*>(enemy_)->idle(150_ms);
                    } else {
                        e_attack = dynamic_cast<gm::Red*>(enemy_)->attack(150_ms);
                    }
                } else if (dynamic_cast<gm::Green*>(enemy_) != nullptr) {
                    if (enemy_pos == eidle) {
                        dynamic_cast<gm::Green*>(enemy_)->idle(150_ms);
                    } else {
                        e_attack = dynamic_cast<gm::Green*>(enemy_)->attack(150_ms);
                    }
                } else if (dynamic_cast<gm::Black*>(enemy_) != nullptr) {
                    if (enemy_pos == eidle) {
                        dynamic_cast<gm::Black*>(enemy_)->idle(150_ms);
                    } else {
                        e_attack = dynamic_cast<gm::Black*>(enemy_)->attack(150_ms);
                    }
                } else {
                    if (enemy_pos == eidle) {
                        dynamic_cast<gm::Troll*>(enemy_)->idle(150_ms);
                    } else if (enemy_pos == eattack) {
                        e_attack = dynamic_cast<gm::Troll*>(enemy_)->attack(82_ms);
                    } else{
                        dynamic_cast<gm::Troll*>(enemy_)->speak(164_ms);
                    }
                }
                enemy_->draw(*this);
                if (e_attack) {
                    enemy_pos = eidle;
                }
            } else {
                spawnEnemy();
            }

            if (q_ != nullptr) {
                q_->draw(*this);
            }
            hero_->draw(*this);

            this->draw(hero_hp);
            if (dis()) {
                this->draw(enemy_hp);
            }
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
                dynamic_cast<gm::Troll*>(enemy_)->idle(75_ms);
            }
            enemy_->draw(*this);
        }
        hero_->draw(*this);

        this->draw(hero_hp);
        if (dis()) {
            this->draw(enemy_hp);
        }
        this->display();
    }
}

void App::spawnEnemy() {
    sf::Texture t;
    t.loadFromFile("../textures/dragons/red.png");

    const auto red = new gm::Red(gm::Model(t), 100, 20);
    t.loadFromFile("../textures/dragons/green_ani.png");
    const auto green = new gm::Green(gm::Model(t), 100, 20);
    t.loadFromFile("../textures/dragons/black_ani.png");
    const auto black = new gm::Black(gm::Model(t), 250, 40);

    t.loadFromFile("../textures/troll.png");
    const auto troll = new gm::Troll(gm::Model(t), 50, 10);

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
    q_ = new Dialog;
    q_->setPosition(enemy_->getPosition().x - 512, enemy_->getPosition().y - 256);
    q_->setLabel(enemy_->question());
}
