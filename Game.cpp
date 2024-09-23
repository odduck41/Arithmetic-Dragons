#include "Game.h"

#include <utility>

sf::Clock gm::Timer::clock_;

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

void gm::Unit::setPosition(const float& x, const float& y) const {
    this->model_->setPosition({x, y});
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
    auto hero_rect = this->model_->getTextureRect();
    ++idle_;
    idle_ %= 9;
    if (idle_ < 6) {
        hero_rect.top = 0;
        hero_rect.left = 32 * (idle_ % 2);
    } else if (idle_ <= 8) {
        hero_rect.top = 32;
        hero_rect.left = 32 * (idle_ % 2);
    }

    this->model_->setTextureRect(hero_rect);
}

void gm::Hero::left(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();

    if (auto sc = model_->getScale(); sc.x > 0) {
        sc.x *= -1;
        model_->setScale(sc);
        model_->setPosition(model_->getPosition() + sf::Vector2f{32, 0});
    }

    auto hero_rect = this->model_->getTextureRect();
    ++run_;
    hero_rect.top = 32 * 3;
    hero_rect.left = 32 * (run_ % 8);

    this->model_->setTextureRect(hero_rect);
}

void gm::Hero::right(const Milliseconds& ms) {
    if (!timer_.passed(ms)) return;
    timer_.update();

    if (auto sc = model_->getScale(); sc.x < 0) {
        sc.x *= -1;
        model_->setScale(sc);
        model_->setPosition(model_->getPosition() - sf::Vector2f{32, 0});
    }

    auto hero_rect = this->model_->getTextureRect();
    ++run_;
    hero_rect.top = 32 * 3;
    hero_rect.left = 32 * (run_ % 8);

    this->model_->setTextureRect(hero_rect);
}

gm::Enemy::Enemy(Model model, const long long hp, const long long attack, const sf::IntRect rect) :
Unit(std::move(model), hp, attack, rect) {}

bool gm::Enemy::isEnemy() const {
    return true;
}

gm::Dragon::Dragon(Model model, const long long hp, const long long attack) :
Enemy(std::move(model), hp, attack) {}
