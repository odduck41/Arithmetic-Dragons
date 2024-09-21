#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

int main() {
    sf::RenderWindow window({928, 793}, "Dragons");

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
    }
    std::ranges::reverse(backgrounds);

    sf::Texture hero_texture;
    hero_texture.loadFromFile("../textures/hero.png");

    sf::Sprite hero(hero_texture);
    hero.setTextureRect({0, 0, 32, 32});
    hero.setPosition(100, 694);
    int hero_run = 0;
    // int hero_idle = 0;

    sf::Clock clock;
    sf::Time last = clock.getElapsedTime();

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (!window.hasFocus()) continue;
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
                && event.type == sf::Event::KeyPressed) {
                ;
            }
        }
        window.clear(sf::Color::White);
        for (auto& back: backgrounds) {
            window.draw(back);
        }
        // if (hero_run == -1) {
        //     if (const sf::Time now = clock.getElapsedTime();
        //         now.asMilliseconds() - last.asMilliseconds() >= 200) {
        //         auto hero_rect = hero.getTextureRect();
        //         ++hero_idle;
        //         hero_idle %= 5;
        //         if (hero_idle < 2) {
        //             hero_rect.top = 0;
        //             hero_rect.left = 32 * hero_idle;
        //         } else if (hero_idle <= 4) {
        //             hero_rect.top = 32;
        //             hero_rect.left = 32 * (hero_idle % 2);
        //         }
        //
        //         hero.setTextureRect(hero_rect);
        //         last = now;
        //     }
        // }
        if (const sf::Time now = clock.getElapsedTime();
        now.asMilliseconds() - last.asMilliseconds() >= 41.667) {
            for (size_t i = 0; i < backgrounds.size(); ++i) {
                auto rect = backgrounds[i].getTextureRect();
                rect.left += (static_cast<int>(i) + 1);
                backgrounds[i].setTextureRect(rect);
            }

            auto hero_rect = hero.getTextureRect();
            hero_rect.top = 32 * 3;
            hero_rect.left = 32 * ((++hero_run) %= 8);
            hero.setTextureRect(hero_rect);
            last = now;
        }
        window.draw(hero);
        window.display();
    }
    return 41;
}
