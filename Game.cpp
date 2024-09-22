#include "Game.h"

#include <utility>

gm::Model::Model(const sf::Texture& texture) : texture_(new sf::Texture(texture)) {}


gm::Model::Model(Model && other) noexcept {
    this->idle_ = other.idle_;
    this->run_ = other.run_;
    this->die_ = other.die_;

    this->texture_ = std::exchange(other.texture_, nullptr);
}

void gm::Model::idle() {
    auto newRect = this->getTextureRect();
    newRect.top = 0;
    newRect.left += 32 * ((++this->idle_) %= 6);
    this->setTextureRect(newRect);
}

void gm::Model::run() {
    auto newRect = this->getTextureRect();
    newRect.top = 32;
    newRect.left += 32 * ((++this->idle_) %= 8);
    this->setTextureRect(newRect);
}

void gm::Model::die() {
    auto newRect = this->getTextureRect();
    newRect.top = 64;
    newRect.left += 32 * ((++this->idle_) %= 12);
    this->setTextureRect(newRect);
}

gm::Model::~Model() override {
    delete this->texture_;
}


gm::Unit::Unit(Model model, const long long hp, const long long attack) :
model_(new Model(std::move(model))), hp_(hp), attack_(attack) {}

bool gm::Unit::isEnemy() const {
    return true;
}

bool gm::Unit::isAlive() const {
    return this->hp_ > 0;
}

void gm::Unit::damage(const unsigned long long & damage) {
    this->hp_ -= static_cast<long long>(damage);
}

void gm::Unit::idle() const {
    this->model_->idle();
}

void gm::Unit::run() const {
    this->model_->run();
}

void gm::Unit::die() const {
    this->model_->die();
}

void gm::Unit::draw(sf::RenderWindow & window) const {
    window.draw(*this->model_);
}

gm::Unit::~Unit() {
    delete model_;
}


