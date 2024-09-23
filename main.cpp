#include "Game.h"
#include <vector>
#include <iostream>

int main() {
    sf::RenderWindow window({928, 600}, "Dragons");

    sf::Texture t;
    t.loadFromFile("../textures/hero.png");
    gm::Hero hero(gm::Model(t), 100, 20);
    hero.setPosition((928. - 32) / 2, 600 - (793 - 696));

    t.loadFromFile("../textures/troll.png");
    gm::Troll troll(gm::Model(t), 150, 30);
    troll.setPosition(700, 600 - (793 - 696));

    gm::smartBg bg;

    enum EventType {
        left,
        right,
        idle,
        die
    } ev = idle;
    bg.fix(troll);
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
        if (ev == idle) {
            hero.idle(175_ms);
        } else if (ev == left) {
            hero.left(41_ms);
            bg.left(41_ms);
        } else if (ev == right) {
            hero.right(41_ms);
            bg.right(41_ms);
        } else {
            hero.attack(82_ms);
        }
        troll.idle(150_ms);

        bg.draw(window);
        troll.draw(window);
        hero.draw(window);

        window.display();
    }
    return 41;
}
