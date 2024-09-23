#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace gm {
    struct Seconds {
      public:
        explicit Seconds(const float seconds) : seconds_(seconds) {};
        friend bool operator>=(const float& a, const Seconds& b) {
            return a > b.seconds_;
        }
      private:
        float seconds_{};
    };

    struct Milliseconds {
      public:
        explicit Milliseconds(const int milliseconds) : milliseconds_(milliseconds) {};
        friend bool operator>=(const int& a, const Milliseconds& b) {
          return a > b.milliseconds_;
        }
      private:
        int milliseconds_{};
    };


    struct Microseconds {
      public:
        explicit Microseconds(const long long microseconds) : microseconds_(microseconds) {};
        friend bool operator>=(const long long& a, const Microseconds& b) {
          return a > b.microseconds_;
        }
      private:
        long long microseconds_{};
    };

    class Timer {
      public:
          bool passed(Seconds&& s) const {
              const sf::Time now = clock_.getElapsedTime();
              return (now.asSeconds() - last_.asSeconds()) >= s;
          }

          bool passed(Milliseconds&& ms) const {
              const sf::Time now = clock_.getElapsedTime();
              return (now.asMilliseconds() - last_.asMilliseconds()) >= ms;
          }

          bool passed(Microseconds&& Ms) const {
              const sf::Time now = clock_.getElapsedTime();
              return (now.asMicroseconds() - last_.asMicroseconds()) >= Ms;
          };

          void update() {
              last_ = clock_.getElapsedTime();
          }

      private:
          static sf::Clock clock_;
          sf::Time last_ = clock_.getElapsedTime();
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
      virtual void idle() = 0;
      virtual ~IIdler() = default;
    };

    struct IRunner {
        virtual void left() = 0;
        virtual void right() = 0;
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
        explicit Unit(Model, long long = 100, long long = 20,
           sf::IntRect = {0, 0, 32, 32});

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
        explicit Hero(Model, long long = 100, long long = 20,
                      sf::IntRect = {0, 0, 32, 32});
        [[nodiscard]] bool isEnemy() const override;

        void idle() override;
        void left() override;
        void right() override;
        void die() override;
      private:
        int idle_{};
        int run_{};
        int die_{};
    };

    class Enemy : public Unit, IIdler, IDying {
      public:
        explicit Enemy(Model, long long = 100, long long = 30,
          sf::IntRect = {0, 0, 32, 32});
        [[nodiscard]] bool isEnemy() const override;
        virtual std::string question() = 0;
        void idle() override;
        void die() override;
      private:
        int idle_{};
        int die_{};
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

    class Troll final : public Enemy, ISpeaker {
      public:
        explicit Troll(Model, long long, long long);
        std::string question() override;
        void speak() override;
      private:
        TrollQuestionType type_{};
        int speak_{};
    };
}

inline gm::Seconds operator""_s(const unsigned long long x) {
  return gm::Seconds(static_cast<float>(x));
}

inline gm::Milliseconds operator""_ms(const unsigned long long x) {
  return gm::Milliseconds(static_cast<int>(x));
}

inline gm::Microseconds operator""_Ms(const unsigned long long x) {
  return gm::Microseconds(static_cast<long long>(x));
}