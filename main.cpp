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
    //
    // sf::Texture hero_texture;
    // hero_texture.loadFromFile("../textures/hero.png");
    //
    // sf::Sprite hero(hero_texture);
    // hero.setTextureRect({0, 32 * 3, 32, 32});
    // hero.setPosition(100, 600 - (793 - 696)); // (928 - 696)
    // int hero_run = 0;
    // int hero_idle = 0;
    // int troll_idle = 0;
    //
    // sf::Texture enemy_texture;
    // enemy_texture.loadFromFile("../textures/troll.png");
    // sf::Sprite enemy(enemy_texture);
    // enemy.setTextureRect({0, 0, 32, 32});
    // enemy.setPosition(928 + 11, 600 - (793 - 696));
    //
    //
    //
    // sf::Clock clock;
    // sf::Time last = clock.getElapsedTime();
    sf::Texture t;
    t.loadFromFile("../textures/hero.png");
    gm::Hero hero(gm::Model(t), 100, 20);
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (!window.hasFocus()) continue;
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

        }
        window.clear();
        for (auto& back: backgrounds) {
            window.draw(back);
        }
        hero.idle();

        hero.draw(window);
        window.display();
    }
    return 41;
}
