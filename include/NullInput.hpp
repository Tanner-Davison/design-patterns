#pragma once
#include "Input.hpp"

/**
 * Null Object Pattern (GoF - Behavioral)
 *
 * NullInput is a do-nothing implementation of the Input interface.
 * It eliminates null-pointer checks inside the Game (Context) by
 * providing a safe, silent default — all inputs return false.
 *
 * Rule of thumb from GoF: "Replace conditional logic for absent objects
 * with a Null Object that provides default do-nothing behaviour."
 *
 * Use this when no real input device is available (e.g. during tests,
 * replays, or cutscenes) rather than passing nullptr and guarding
 * every call site.
 */
class NullInput : public Input {
  public:
    bool Up() override {
        return false;
    }
    bool Down() override {
        return false;
    }
    bool Left() override {
        return false;
    }
    bool Right() override {
        return false;
    }
    ~NullInput() override = default;
};
