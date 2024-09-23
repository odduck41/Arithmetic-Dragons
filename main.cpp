#include "Game.h"
#include <vector>
#include <iostream>

int main() {
    sf::RenderWindow window({928, 600}, "Dragons");

    std::vector<sf::Texture> textures(11);
    std::vector<sf::Sprite> backgrounds(11);

    for (size_t i = 0; i < backgrounds.size(); ++i) {
        sf::Texture texture;
        if (i < 10) {
            texture.loadFromFile("../textures/bgs/Layer_000" + std::to_string(i) + ".png");
        } else {
            texture.loadFromFile("../textures/bgs/Layer_00" + std::to_string(i) + ".png");
        }
        texture.setRepeated(true);
        textures[i] = texture;
        backgrounds[i].setTexture(textures[i]);
        backgrounds[i].setPosition({0, 600 - 793});
    }
    std::ranges::reverse(backgrounds);

    sf::Texture t;
    t.loadFromFile("../textures/hero.png");

    gm::Hero hero(gm::Model(t), 100, 20);
    hero.setPosition((928. - 32) / 2, 600 - (793 - 696));
    t.loadFromFile("../textures/dragons/red.png");
    gm::Red red(gm::Model(t), 150, 30);
    red.setPosition(700, 600 - (793 - 696));

    enum EventType {
        left,
        right,
        idle,
        die
    } ev = idle;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (!window.hasFocus()) continue;
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                ev = right;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                ev = left;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                ev = die;
            } else {
                ev = idle;
            }
        }
        window.clear();
        for (auto& back: backgrounds) {
            window.draw(back);
        }
        if (ev == idle) {
            hero.idle(175_ms);
        } else if (ev == left) {
            // for (size_t i = 0; i < backgrounds.size(); ++i) {
            //     auto rect = backgrounds[i].getTextureRect();
            //     rect.left += (static_cast<int>(i) + 1);
            //     backgrounds[i].setTextureRect(rect);
            // }
            hero.left(41_ms);
        } else if (ev == right) {
            hero.right(41_ms);
        } else {
            hero.die(300_ms);
        }
        red.idle(200_ms);

        hero.draw(window);
        red.draw(window);
        window.display();
    }
    return 41;
}
