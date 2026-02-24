#pragma once

/**
 * Strategy Pattern (GoF - Behavioral)
 *
 * Input is the Strategy interface. It declares the family of algorithms
 * (input handling) that can be swapped at runtime. The Game (Context)
 * delegates all input queries to whichever concrete strategy is injected,
 * without knowing its type.
 *
 * This also serves as the Target interface for the Adapter pattern —
 * any device (Keyboard, Gamepad, AI controller, etc.) can be wrapped to
 * satisfy this interface without touching the Game logic.
 */
class Input {
  public:
    virtual bool Up()    = 0;
    virtual bool Down()  = 0;
    virtual bool Left()  = 0;
    virtual bool Right() = 0;
    virtual ~Input()     = default;
};
