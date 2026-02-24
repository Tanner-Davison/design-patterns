#pragma once
#include "Input.hpp"
#include <random>

/**
 * Concrete Strategy (GoF - Strategy / Behavioral)
 *
 * Keyboard is a concrete Strategy that implements the Input interface.
 * It simulates keyboard input using a Bernoulli distribution.
 * The Game (Context) is unaware of this class — it only holds an Input*.
 */
class Keyboard : public Input {
    std::default_random_engine m_Engine{12345};
    bool                       SimulateInput();

  public:
    bool Up() override;
    bool Down() override;
    bool Left() override;
    bool Right() override;
    ~Keyboard() override = default;
};
