# Strategy Pattern

## What Is It?

The Strategy pattern is a **behavioral design pattern** that defines a family of algorithms, encapsulates each one as an object, and makes them interchangeable. The object that uses the algorithm (the **Context**) doesn't know or care which concrete implementation it's using — it just calls the interface.

Think of it like a GPS navigation app: the destination is fixed, but you can swap the route algorithm (fastest, shortest, avoid tolls) at any time without changing the app itself.

---

## When To Use It

- You have multiple variants of an algorithm and want to switch between them at runtime
- You want to eliminate large `if/else` or `switch` chains that select behaviour
- You want to isolate the business logic of a class from implementation details of its algorithms
- You need different classes that differ only in the way they execute some behaviour

---

## Structure

```
┌─────────────────────────────────────────────────────────────┐
│                         Client                              │
└──────────────────────────────┬──────────────────────────────┘
                               │ creates & injects
                               ▼
┌──────────────────────────────────────────────────────────────┐
│                         Context                              │
│  - strategy_ : Strategy*                                     │
│  + SetStrategy(Strategy*)                                    │
│  + ExecuteStrategy()  ──────────────────► strategy_->Do()   │
└──────────────────────────────────────────────────────────────┘
                               ▲
                               │ implements
          ┌────────────────────┼────────────────────┐
          │                    │                    │
┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
│ ConcreteStrat A │  │ ConcreteStrat B │  │ ConcreteStrat C │
│  + Do()         │  │  + Do()         │  │  + Do()         │
└─────────────────┘  └─────────────────┘  └─────────────────┘
```

---

## Participants

| Participant          | Role                                                        |
| -------------------- | ----------------------------------------------------------- |
| **Strategy**         | Interface declaring the algorithm operation                 |
| **ConcreteStrategy** | A specific implementation of the algorithm                  |
| **Context**          | Holds a reference to a Strategy; delegates work to it       |
| **Client**           | Creates a concrete strategy and injects it into the context |

---

## C++ Implementation

### This Project's Example

```cpp
// Strategy interface — Input.hpp
class Input {
public:
    virtual bool Up()    = 0;
    virtual bool Down()  = 0;
    virtual bool Left()  = 0;
    virtual bool Right() = 0;
    virtual ~Input()     = default;
};

// Concrete Strategy A — Keyboard
class Keyboard : public Input {
    bool Up()    override { /* simulate */ }
    bool Down()  override { /* simulate */ }
    bool Left()  override { /* simulate */ }
    bool Right() override { /* simulate */ }
};

// Concrete Strategy B — GameController
class GameController : public Input {
    bool Up()    override { /* simulate */ }
    bool Down()  override { /* simulate */ }
    bool Left()  override { /* simulate */ }
    bool Right() override { /* simulate */ }
};

// Context — Game() delegates all input queries to the injected strategy.
// Zero knowledge of Keyboard or GameController.
void Game(Input& input) {
    if (input.Up())   { /* pitch up   */ }
    if (input.Left()) { /* roll left  */ }
}

// Client — picks a strategy and injects it
int main() {
    Keyboard k;
    Game(k); // swap to GameController with no changes to Game()
}
```

---

### Classic Conceptual Example

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

// Strategy interface
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual std::string sort(std::string_view data) const = 0;
};

// Concrete Strategy A — ascending sort
class AscendingSort : public SortStrategy {
public:
    std::string sort(std::string_view data) const override {
        std::string result{data};
        std::sort(result.begin(), result.end());
        return result;
    }
};

// Concrete Strategy B — descending sort
class DescendingSort : public SortStrategy {
public:
    std::string sort(std::string_view data) const override {
        std::string result{data};
        std::sort(result.begin(), result.end(), std::greater<>{});
        return result;
    }
};

// Context
class Sorter {
    std::unique_ptr<SortStrategy> m_strategy;
public:
    explicit Sorter(std::unique_ptr<SortStrategy> s) : m_strategy{std::move(s)} {}

    void setStrategy(std::unique_ptr<SortStrategy> s) {
        m_strategy = std::move(s);
    }

    void doSort(std::string_view data) const {
        std::cout << m_strategy->sort(data) << '\n';
    }
};

int main() {
    Sorter sorter{std::make_unique<AscendingSort>()};
    sorter.doSort("dcbea");  // → abcde

    sorter.setStrategy(std::make_unique<DescendingSort>());
    sorter.doSort("dcbea");  // → edcba
}
```

---

## Strategy vs Replacing `if/else`

```cpp
// ❌ Bad — if/else hardcodes every variant inside the context
void Game(InputType type) {
    if (type == InputType::Keyboard) {
        if (keyboardUp())  { /* pitch up */ }
        if (keyboardLeft()){ /* roll    */ }
    } else if (type == InputType::GameController) {
        if (padUp())  { /* pitch up */ }
        if (padLeft()){ /* roll    */ }
    }
    // adding a third device = edit this function again ❌
}

// ✅ Good — context is closed for modification, open for extension
void Game(Input& input) {
    if (input.Up())   { /* pitch up */ }
    if (input.Left()) { /* roll    */ }
    // adding a third device = new class only, Game() untouched ✅
}
```

---

## Runtime Strategy Swapping

One of Strategy's key strengths is swapping behaviour **while the program is running**:

```cpp
// Context holds a unique_ptr so the strategy can be replaced at runtime
class Game {
    std::unique_ptr<Input> m_input;
public:
    explicit Game(std::unique_ptr<Input> input) : m_input{std::move(input)} {}

    void setInput(std::unique_ptr<Input> input) {
        m_input = std::move(input);
    }

    void update() {
        if (m_input->Up())   { /* pitch up   */ }
        if (m_input->Left()) { /* roll left  */ }
    }
};

int main() {
    Game game{std::make_unique<Keyboard>()};
    game.update();

    // Hot-swap to a gamepad mid-session
    game.setInput(std::make_unique<GameController>());
    game.update();
}
```

---

## Strategy vs Similar Patterns

| Pattern             | Intent                                                                                |
| ------------------- | ------------------------------------------------------------------------------------- |
| **Strategy**        | Swaps the _entire algorithm_ behind an interface                                      |
| **State**           | Changes behaviour by switching internal state (strategies that know about each other) |
| **Template Method** | Defines the algorithm skeleton in a base class; subclasses fill in steps              |
| **Command**         | Encapsulates a _request_ as an object; Strategy encapsulates an _algorithm_           |
| **Decorator**       | Adds behaviour _around_ an object without replacing its algorithm                     |

> Key distinction: **Strategy** replaces the whole algorithm. **Decorator** wraps it. **Template Method** fixes the skeleton and lets subclasses vary the steps.

---

## Pros and Cons

### Pros

- Follows the **Open/Closed Principle** — add new strategies without touching the context
- Follows the **Single Responsibility Principle** — each strategy class does one thing
- Eliminates conditionals that select behaviour
- Strategies can be swapped at runtime
- Easy to test strategies in isolation

### Cons

- Clients must be aware of the differences between strategies to pick the right one
- If you only have two or three strategies that never change, simpler `if/else` may be clearer
- Adds extra classes and indirection for trivial cases

---

## Common Mistakes

```cpp
// ❌ Bad — context knows concrete strategy types (defeats the purpose)
void Game(Input* pInput) {
    if (auto* k = dynamic_cast<Keyboard*>(pInput)) {
        // keyboard-specific logic
    }
}

// ✅ Good — context only ever calls the interface
void Game(Input& input) {
    if (input.Up()) { /* ... */ }
}
```

```cpp
// ❌ Bad — strategy holds a back-reference to the context (tight coupling)
class Keyboard : public Input {
    Game* m_game; // now strategy and context are coupled to each other
};

// ✅ Good — strategy is self-contained, context passes data via method args
class Keyboard : public Input {
    bool Up() override { return SimulateInput(); }
};
```

---

## Quick Reference

```
Problem:  You have a class that needs to do X, but there are
          multiple ways to do X and you want to switch between them.

Solution: Extract each way into its own Strategy class that
          implements a common interface. Inject the strategy
          into the Context via constructor or setter.

Rule of thumb:
  - Behaviour varies by type?       → Strategy
  - Behaviour varies by state?      → State
  - Skeleton fixed, steps vary?     → Template Method
  - One variant that never changes? → Inline it (YAGNI)
```

---

## See Also

- `docs/Adapter.md` — making incompatible interfaces work together
- `docs/FactoryMethod.md` — creating the right concrete strategy without `new` in client code
- `docs/NullObject.md` _(planned)_ — a strategy that safely does nothing
