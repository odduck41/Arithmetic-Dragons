#pragma once
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace gm {

    extern std::mt19937 generator;

    struct Milliseconds {
      public:
        explicit Milliseconds(const int milliseconds) : milliseconds_(milliseconds) {};
        friend bool operator>=(const int& a, const Milliseconds& b) {
          return a > b.milliseconds_;
        }
      private:
        int milliseconds_{};
    };

    class Timer {
      public:
          Timer() {
              last_ = clock_.getElapsedTime();;
          }
          [[nodiscard]] bool passed(const Milliseconds& ms) const {
              const sf::Time now = clock_.getElapsedTime();
              return (now.asMilliseconds() - last_.asMilliseconds()) >= ms;
          }

          void update() {
              last_ = clock_.getElapsedTime();
          }

      private:
          static sf::Clock clock_;
          sf::Time last_{};
    };

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
      virtual void idle(const Milliseconds&) = 0;
      virtual ~IIdler() = default;
    };

    struct IRunner {
        virtual void left(const Milliseconds&) = 0;
        virtual void right(const Milliseconds&) = 0;
        virtual ~IRunner() = default;
    };

    struct IDying {
        virtual void die(const Milliseconds&) = 0;
        virtual ~IDying() = default;
    };

    struct ISpeaker {
      virtual void speak(const Milliseconds&) = 0;
      virtual ~ISpeaker() = default;
    };

    struct IAttacker {
        virtual void attack(const Milliseconds&) = 0;
        virtual ~IAttacker() = default;
    };

    class Unit {
      public:
        explicit Unit(Model, long long = 100, long long = 20,
           sf::IntRect = {0, 0, 32, 32});

        [[nodiscard]] virtual bool isEnemy() const = 0;

        [[nodiscard]] bool isAlive() const;

        void damage(const unsigned long long&);
        void attack(Unit&) const;

        void draw(sf::RenderWindow&) const;
        void setPosition(const float&, const float&) const;

        virtual ~Unit();

      protected:
        Model* model_{};
        Timer timer_{};
      private:
        long long hp_ = 100;
        long long attack_ = 20;
    };

    class Hero final : public Unit, IIdler, IRunner, IDying, IAttacker {
      public:
        explicit Hero(Model, long long = 100, long long = 20,
                      sf::IntRect = {0, 0, 32, 32});
        [[nodiscard]] bool isEnemy() const override;

        void idle(const Milliseconds&) override;
        void left(const Milliseconds&) override;
        void right(const Milliseconds&) override;
        void die(const Milliseconds&) override;
        void attack(const Milliseconds&) override;
      private:
        int idle_{};
        int run_{};
        int die_{};
        int atck_{};
    };

    class Enemy : public Unit, IIdler, IDying, IAttacker {
      public:
        explicit Enemy(Model, long long = 100, long long = 30,
          sf::IntRect = {0, 0, 32, 32});
        [[nodiscard]] bool isEnemy() const override;
        virtual std::string question() = 0;
      protected:
        int idle_{};
        int die_{};
    };

    class Dragon : public Enemy {
      public:
        explicit Dragon(Model, long long, long long);
        [[nodiscard]] virtual bool answer(long long) const = 0;
        void idle(const Milliseconds&) override;
        void die(const Milliseconds&) override;
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
        void die(const Milliseconds&) override;
    };

    enum TrollQuestionType {
        guessing,
        prime,
        odd,
        even
    };

    class Troll final : public Enemy, ISpeaker {
      public:
        explicit Troll(Model, long long, long long);
        std::string question() override {;};

        void idle(const Milliseconds&) override;
        void die(const Milliseconds&) override;
        void speak(const Milliseconds&) override;
      private:
        TrollQuestionType type_{};
        int speak_{};
    };
}

inline gm::Milliseconds operator""_ms(const unsigned long long x) {
    return gm::Milliseconds(static_cast<int>(x));
}
