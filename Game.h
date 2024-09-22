#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace gm {
    class Model final : public sf::Sprite {
      public:
        explicit Model(const sf::Texture&);

        Model(const Model&);
        Model(Model&&) noexcept;
        Model& operator=(Model);

        ~Model() override;
      private:
        sf::Texture* texture_{};
    };

    struct IIdler {
      virtual void idle() = 0;
      virtual ~IIdler() = default;
    };

    struct IRunner {
        virtual void run() = 0;
        virtual ~IRunner() = default;
    };

    struct IDying {
        virtual void die() = 0;
        virtual ~IDying() = default;
    };

    struct ISpeaker {
      virtual void speak() = 0;
      virtual ~ISpeaker() = default;
    };

    class Unit {
      public:
        explicit Unit(Model, long long = 100, long long = 20);

        [[nodiscard]] virtual bool isEnemy() const = 0;

        [[nodiscard]] bool isAlive() const;

        void damage(const unsigned long long&);
        void attack(Unit&) const;

        void draw(sf::RenderWindow&) const;
        virtual ~Unit();

      protected:
        Model* model_{};
      private:
        long long hp_ = 100;
        long long attack_ = 20;
    };

    class Hero final : public Unit, IIdler, IRunner, IDying {
      public:
        Hero(Model, long long, long long);
        [[nodiscard]] bool isEnemy() const override;

        void idle() override;
        void run() override;
        void die() override;
      private:
        int idle_{};
        int run_{};
        int die_{};
    };

    class Enemy : public Unit, IIdler, IDying, ISpeaker {
      public:
        explicit Enemy(Model, long long, long long);
        [[nodiscard]] bool isEnemy() const override;
        virtual std::string question() = 0;
        void idle() override;
        void die() override;
      private:
        int idle_{};
        int die_{};
        int speak_{};
    };

    class Dragon : Enemy {
      public:
        explicit Dragon(Model, long long, long long);
        [[nodiscard]] virtual bool answer(long long) const = 0;
      protected:
        long long a{};
        long long b{};
    };

    class Red final : public Dragon { // Red asks the sum
      public:
        explicit Red(Model, long long, long long);
        std::string question() override;
        [[nodiscard]] bool answer(long long) const override;
    };

    class Green final : public Dragon { // Green asks the difference
      public:
        explicit Green(Model, long long, long long);
        std::string question() override;
        [[nodiscard]] bool answer(long long) const override;
    };

    class Black final : public Dragon { // Black asks the multiplication
      public:
        explicit Black(Model, long long, long long);
        std::string question() override;
        [[nodiscard]] bool answer(long long) const override;
    };

    enum TrollQuestionType {
      guessing,
      prime,
      odd,
      even
    };

    class Troll final : public Enemy {
      public:
        explicit Troll(Model, long long, long long);
        std::string question() override;
      private:
        TrollQuestionType type_{};
    };
}
