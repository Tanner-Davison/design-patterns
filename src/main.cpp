/**
 * ============================================================
 *  Applied GoF Design Patterns
 * ============================================================
 *
 *  1. STRATEGY (Behavioral)
 *     Input       = Strategy interface
 *     Keyboard    = Concrete Strategy A
 *     GameController = Concrete Strategy B
 *     Game()      = Context  — delegates all input queries to
 *                   whichever Input* is injected; zero knowledge
 *                   of the concrete type.
 *
 *  2. FACTORY METHOD (Creational)
 *     InputFactory::Create() = Factory method
 *     main() never calls `new Keyboard` / `new GameController`
 *     directly — it asks the factory for an "Input by name".
 *     Adding a new device = add one class + one factory branch.
 *
 *  3. NULL OBJECT (Behavioral / GoF structural variant)
 *     NullInput = do-nothing Input that returns false for every
 *     query. Passed as the fallback so Game() never receives a
 *     nullptr and never needs null-checks.
 *
 * ============================================================
 */
#include "Input.hpp"
#include "InputFactory.hpp"
#include <print>
#include <thread>
using namespace std::literals;

// ------------------------------------------------------------------
// Context (Strategy pattern)
// Game only depends on the Input interface — it is completely
// decoupled from Keyboard, GameController, NullInput, etc.
// ------------------------------------------------------------------
void Game(Input& input) {
    int count{5};
    while (count != 0) {
        std::print("===================\n");

        // Pitch axis
        if (input.Up()) {
            std::print("Pitch up\n");
        } else if (input.Down()) {
            std::print("Pitch down\n");
        } else {
            std::print("Plane is level\n");
        }

        // Roll axis
        if (input.Left()) {
            std::print("Roll left\n");
        } else if (input.Right()) {
            std::print("Roll right\n");
        } else {
            std::print("Plane is flying straight\n");
        }

        std::print("\n");
        std::this_thread::sleep_for(1s);
        --count;
    }
}

int main() {
    // Factory Method — client asks for a device by name.
    // Swap "keyboard" → "gamecontroller" or "null" here without
    // touching Game() or any other call site.
    auto input = InputFactory::Create("keyboard");

    // Strategy — pass the concrete strategy as the abstract Input&.
    // Game() never sees the type; it only calls the interface.
    Game(*input);
}
