#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace gm {
    class Model final : public sf::Sprite {
      public:
        explicit Model(const sf::Texture&);

        Model(Model&&) noexcept;

        void idle();
        void run();
        void die();

        ~Model() override;
      private:
        sf::Texture* texture_{};
        int idle_{};
        int run_{};
        int die_{};
    };

    class Unit {
      public:
        explicit Unit(Model, long long = 100, long long = 20);

        [[nodiscard]] virtual bool isEnemy() const = 0;

        [[nodiscard]] bool isAlive() const;

        void damage(const unsigned long long&);

        void idle() const;
        void run() const;
        void die() const;

        void draw(sf::RenderWindow&) const;
        virtual ~Unit();

      protected:
        Model* model_{};
      private:
        long long hp_ = 100;
        long long attack_ = 20;
    };

    class Hero final : public Unit {
      public:
        Hero(Model, long long, long long);
        [[nodiscard]] bool isEnemy() const override;
    };

    class Enemy : public Unit {
      public:
        explicit Enemy(Model, long long, long long);
    };

    class Dragon : Enemy {
      public:
        explicit Dragon(Model, long long, long long);
    };

    class Red final : public Dragon {
      public:
        explicit Red(Model, long long, long long);
        [[nodiscard]] bool isEnemy() const override;
    };

    class Green final : public Dragon {
      public:
        explicit Green(Model, long long, long long);
        [[nodiscard]] bool isEnemy() const override;
    };

    class Black final : public Dragon {
      public:
        explicit Green(Model, long long, long long);
        [[nodiscard]] bool isEnemy() const override;
    };
}
