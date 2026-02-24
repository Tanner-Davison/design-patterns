#pragma once
#include "GameController.hpp"
#include "Input.hpp"
#include "Keyboard.hpp"
#include "NullInput.hpp"
#include <memory>
#include <stdexcept>
#include <string>

/**
 * Factory Method Pattern (GoF - Creational)
 *
 * InputFactory decouples object creation from the client (main).
 * The client asks for an Input by name — it never calls `new Keyboard`
 * or `new GameController` directly. Adding a new input device means
 * adding a new branch here and a new concrete class; no other code changes.
 *
 * GoF intent: "Define an interface for creating an object, but let
 * subclasses decide which class to instantiate."
 *
 * Here we use the simple (static) factory variant — appropriate when
 * there is only one creator and no need to subclass the factory itself.
 * Promote to a full Creator/ConcreteCreator hierarchy if that changes.
 */
class InputFactory {
  public:
    // Supported device names: "keyboard", "gamecontroller", "null"
    static std::unique_ptr<Input> Create(const std::string& device) {
        if (device == "keyboard")
            return std::make_unique<Keyboard>();
        if (device == "gamecontroller")
            return std::make_unique<GameController>();
        if (device == "null")
            return std::make_unique<NullInput>();
        throw std::invalid_argument{"InputFactory: unknown device '" + device + "'"};
    }
};
