#include "Input.hpp"
#include "Keyboard.hpp"
#include <print>
#include <thread>
using namespace std::literals;
void Game(Input* pInput) {
    int count{5};
    while (count != 0) {
        std::print("===================\n");
        if (pInput->Up()) {
            std::print("Pitch up\n");
        } else if (pInput->Down()) {
            std::print("Pitch down\n");
        } else {
            std::print("Plane is level\n");
        }
        if (pInput->Left()) {
            std::print("Roll left\n");
        } else if (pInput->Right()) {
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
    Keyboard k;
    Game(&k);
}
