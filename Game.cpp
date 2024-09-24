#include "Game.h"

#include <ctime>
#include <random>
#include <utility>

sf::Clock gm::Timer::clock_;
std::mt19937 gm::generator(std::time(new std::time_t));

gm::Model::Model(const sf::Texture& texture) : texture_(new sf::Texture(texture)) {
    this->setTexture(texture);
}

gm::Model::Model(const Model& other) : Sprite(other) {
    if (other.texture_ == nullptr) {
        this->texture_ = nullptr;
        return;
    }
    this->texture_ = new sf::Texture;
    *this->texture_ = *other.texture_;
}


gm::Model::Model(Model&& other) noexcept {
    this->texture_ = std::exchange(other.texture_, nullptr);
    this->setTexture(*this->texture_);
}

gm::Model& gm::Model::operator=(Model other) {
    std::swap(*this, other);
    return *this;
}

gm::Model::~Model() {
    delete this->texture_;
}


gm::Unit::Unit(Model model, const long long hp, const long long attack, const sf::IntRect rect) :
model_(new Model(std::move(model))), hp_(hp), attack_(attack) {
    model_->setTextureRect(rect);
}

bool gm::Unit::isEnemy() const {
    return true;
}

bool gm::Unit::isAlive() const {
    return this->hp_ > 0;
}

void gm::Unit::damage(const unsigned long long & damage) {
    this->hp_ -= static_cast<long long>(damage);
}

void gm::Unit::attack(Unit& other) const {
    other.damage(this->attack_);
}

void gm::Unit::draw(sf::RenderWindow & window) const {
    window.draw(*this->model_);
}

sf::Vector2f gm::Unit::getPosition() const {
    return this->model_->getPosition();
}

void gm::Unit::setPosition(const float& x, const float& y) const {
    this->model_->setPosition({x, y});
}

void gm::Unit::setPosition(const sf::Vector2f& pos) const {
    this->model_->setPosition(pos);
}

gm::Unit::~Unit() {
    delete model_;
}


gm::Hero::Hero(Model model, const long long hp, const long long attack, const sf::IntRect rect) :
Unit(std::move(model), hp, attack, rect) {}

bool gm::Hero::isEnemy() const {
    return false;
}

void gm::Hero::idle(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();
    auto rect = this->model_->getTextureRect();
    idle_ %= 9;
    if (idle_ < 6) {
        rect.top = 0;
        rect.left = 32 * (idle_ % 2);
    } else if (idle_ <= 8) {
        rect.top = 32;
        rect.left = 32 * (idle_ % 2);
    }
    ++idle_;

    this->model_->setTextureRect(rect);
}

void gm::Hero::left(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();

    if (auto sc = model_->getScale(); sc.x > 0) {
        sc.x *= -1;
        model_->setScale(sc);
        model_->setPosition(model_->getPosition() + sf::Vector2f{32, 0});
    }

    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 3;
    rect.left = 32 * (run_ %= 8);
    ++run_;

    this->model_->setTextureRect(rect);
}

void gm::Hero::right(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();

    if (auto sc = model_->getScale(); sc.x < 0) {
        sc.x *= -1;
        model_->setScale(sc);
        model_->setPosition(model_->getPosition() - sf::Vector2f{32, 0});
    }

    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 3;
    rect.left = 32 * (run_ %= 8);
    ++run_;

    this->model_->setTextureRect(rect);
}

bool gm::Hero::die(const Milliseconds& ms) {
    if (die_ == 4) return true;
    if (!timer_.passed(ms)) return false;
    timer_.update();
    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 6;
    rect.left = 32 * (die_);
    ++die_;

    this->model_->setTextureRect(rect);
    return false;
}

void gm::Hero::attack(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();
    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 8;
    rect.left = 32 * (atck_ %= 8);
    ++atck_;

    this->model_->setTextureRect(rect);
}

gm::Enemy::Enemy(Model model, const long long hp, const long long attack, const sf::IntRect rect) :
Unit(std::move(model), hp, attack, rect) {}

bool gm::Enemy::isEnemy() const {
    return true;
}

gm::Dragon::Dragon(Model model, const long long hp, const long long attack) :
Enemy(std::move(model), hp, attack) {}

void gm::Dragon::idle(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.left = 32 * (idle_ %= 4);
    ++idle_;

    this->model_->setTextureRect(rect);
}

bool gm::Dragon::die(const Milliseconds& ms) {
    if (die_ >= 9) return true;
    if (!timer_.passed(ms)) return false;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.top = 32;
    rect.left = 32 * (die_);
    ++die_;

    this->model_->setTextureRect(rect);
    return false;
}

void gm::Dragon::attack(const Milliseconds& ms) {
    if (!timer_.passed(ms) || die_ >= 9) return;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 2;
    rect.left = 32 * (atck_ %= 6);
    ++atck_;

    this->model_->setTextureRect(rect);
}

gm::Red::Red(Model model, const long long hp, const long long attack)
: Dragon(std::move(model), hp, attack) {}

std::string gm::Red::question() {
    this->a = generator() % 100;
    this->b = generator() % 100;
    return "Give me a sum of " + std::to_string(a) + " and "
    + std::to_string(b) + "!";
}

bool gm::Red::answer(const long long ans) const {
    return ans == a + b;
}

gm::Green::Green(Model model, const long long hp, const long long attack)
: Dragon(std::move(model), hp, attack) {}

std::string gm::Green::question() {
    this->a = generator() % 100;
    this->b = generator() % 100;
    return "Give me a difference between " + std::to_string(a) + " and "
    + std::to_string(b) + "!";
}

bool gm::Green::answer(const long long ans) const {
    return ans == a - b;
}

gm::Black::Black(Model model, const long long hp, const long long attack)
: Dragon(std::move(model), hp, attack) {}

std::string gm::Black::question() {
    this->a = generator() % 100;
    this->b = generator() % 100;
    return "Give me a multiplication of " + std::to_string(a) + " and "
    + std::to_string(b) + "!";
}

bool gm::Black::answer(const long long ans) const {
    return ans == a * b;
}

bool gm::Black::die(const Milliseconds &ms) {
    if (die_ >= 12) return true;
    if (!timer_.passed(ms)) return false;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.top = 32;
    rect.left = 32 * (die_);
    ++die_;

    this->model_->setTextureRect(rect);
    return false;
}

gm::Troll::Troll(Model model, const long long hp, const long long attack) :
Enemy(std::move(model), hp, attack) {}

void gm::Troll::idle(const Milliseconds& ms) { // 160
    if (!timer_.passed(ms)) return;
    timer_.update();
    auto rect = this->model_->getTextureRect();
    idle_ %= 7;
    if (idle_ < 4) {
        rect.top = 0;
        rect.left = 32 * (idle_ % 2);
    } else if (idle_ <= 6) {
        rect.top = 32;
        rect.left = 32 * (idle_ % 2);
    }
    ++idle_;

    this->model_->setTextureRect(rect);
}

bool gm::Troll::die(const Milliseconds& ms) { // 150
    if (die_ > 12) return true;
    if (!timer_.passed(ms)) return false;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 3;
    rect.left = 32 * (die_);
    ++die_;

    this->model_->setTextureRect(rect);
    return false;
}

void gm::Troll::speak(const Milliseconds& ms) { // 75
    if (!timer_.passed(ms)) return;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 2;
    rect.left = 32 * (speak_ %= 4);
    ++speak_;

    this->model_->setTextureRect(rect);
}

void gm::Troll::attack(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();

    auto rect = this->model_->getTextureRect();
    rect.top = 32 * 4;
    rect.left = 32 * (atck_ %= 16);
    ++atck_;

    this->model_->setTextureRect(rect);
}

bool gm::Troll::answer(const long long a) const {
    if (type_ == guessing) {
        return a_ == a;
    }
    if (type_ == odd) {
        return a % 2;
    }
    return a % 2 == 0;
}


std::string gm::Troll::question() {
    const auto e = generator() % 3;
    if (e == 0) {
        type_ = guessing;
        a_ = generator() % 5;
        return "Guess number from 0 to 5!";
    }
    if (e == 1) {
        type_ = odd;
        return "Give me an odd number!";
    }
    return "Give me an even number!";
}


gm::smartBg::smartBg()  {
    backgrounds.resize(11);
    textures.resize(11);
    for (size_t i = 0; i < backgrounds.size(); ++i) {
        sf::Texture texture;
        if (i < 10) {
            texture.loadFromFile("../textures/bgs/Layer_000"
              + std::to_string(i) + ".png");
        } else {
            texture.loadFromFile("../textures/bgs/Layer_00"
              + std::to_string(i) + ".png");
        }
        texture.setRepeated(true);
        textures[i] = texture;
        backgrounds[i].setTexture(textures[i]);
        backgrounds[i].setPosition({0, 600 - 793});
    }
    std::ranges::reverse(backgrounds);
}

void gm::smartBg::left(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();
    for (size_t i = 0; i < backgrounds.size(); ++i) {
        auto rect = backgrounds[i].getTextureRect();
        rect.left -= (static_cast<int>(i) + 1);
        backgrounds[i].setTextureRect(rect);
    }

    for (const auto& fixed: fixed_) {
        fixed->setPosition(fixed->getPosition() + sf::Vector2f(11, 0));
    }
}

void gm::smartBg::right(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();
    for (size_t i = 0; i < backgrounds.size(); ++i) {
        auto rect = backgrounds[i].getTextureRect();
        rect.left += (static_cast<int>(i) + 1);
        backgrounds[i].setTextureRect(rect);
    }

    for (const auto& fixed: fixed_) {
        fixed->setPosition(fixed->getPosition() - sf::Vector2f(11, 0));
    }
}

void gm::smartBg::draw(sf::RenderWindow& window) {
    for (auto& back: backgrounds) {
        window.draw(back);
    }
}

void gm::smartBg::fix(Unit& obj) {
    fixed_.push_back(&obj);
}

