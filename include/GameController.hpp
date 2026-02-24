#pragma once
#include "Input.hpp"
#include <random>

/**
 * Concrete Strategy (GoF - Strategy / Behavioral)
 *
 * GameController is a second concrete Input strategy that simulates
 * analogue gamepad input using a different probability distribution.
 * Swapping from Keyboard to GameController requires zero changes to
 * the Game (Context) — only the strategy injected into it changes.
 */
class GameController : public Input {
    std::default_random_engine  m_Engine{99999};
    std::bernoulli_distribution m_Dist{0.3}; // less twitchy than a keyboard

    bool SimulateInput();

  public:
    bool Up() override;
    bool Down() override;
    bool Left() override;
    bool Right() override;
    ~GameController() override = default;
};
