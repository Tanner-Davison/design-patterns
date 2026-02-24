#include "GameController.hpp"

bool GameController::SimulateInput() {
    return m_Dist(m_Engine);
}
bool GameController::Up() {
    return SimulateInput();
}
bool GameController::Down() {
    return SimulateInput();
}
bool GameController::Left() {
    return SimulateInput();
}
bool GameController::Right() {
    return SimulateInput();
}
