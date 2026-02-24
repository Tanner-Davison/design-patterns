# Factory Method Pattern

## What Is It?

The Factory Method pattern is a **creational design pattern** that defines an interface for creating an object, but lets subclasses (or a dedicated factory) decide which class to instantiate. The client asks for a product by type — it never calls `new ConcreteClass()` directly.

Think of it like ordering from a restaurant: you say "I'll have the pasta" — you don't go into the kitchen and assemble the dish yourself. The kitchen (factory) handles construction; you receive the finished product through a common interface.

---

## When To Use It

- You don't know ahead of time which class you need to instantiate
- You want to decouple object creation from the code that uses the object
- You want a single place to change which concrete class gets created (Open/Closed)
- You're building a framework and want users to extend it by supplying their own product types

---

## Structure

### Full GoF Form (Creator hierarchy)

```
┌──────────────────────────────┐
│         Creator              │  ← declares FactoryMethod()
│  + FactoryMethod() : Product*│
│  + SomeOperation()           │  ← uses the product
└──────────────┬───────────────┘
               │ subclasses override
    ┌──────────┴───────────┐
    │                      │
┌───────────────┐  ┌────────────────┐
│ConcreteCreatorA│  │ConcreteCreatorB│
│+FactoryMethod()│  │+FactoryMethod()|
│  → ProductA    │  │  → ProductB    │
└───────────────┘  └────────────────┘
         │                   │
         ▼                   ▼
    ┌─────────┐         ┌─────────┐
    │ProductA │         │ProductB │  both implement Product interface
    └─────────┘         └─────────┘
```

### Simple Factory Variant (used in this project)

```
┌──────────────────────────────────────────────────┐
│               InputFactory                       │
│  + Create(device: string) : unique_ptr<Input>    │
│    "keyboard"       → Keyboard                   │
│    "gamecontroller" → GameController             │
│    "null"           → NullInput                  │
└──────────────────────────────────────────────────┘
                         │
                         ▼
                   ┌───────────┐
                   │   Input   │  ← Product interface
                   └───────────┘
```

---

## Participants

| Participant         | Role                                                              |
| ------------------- | ----------------------------------------------------------------- |
| **Product**         | The interface all created objects implement                       |
| **ConcreteProduct** | A specific product class (`Keyboard`, `GameController`)           |
| **Creator**         | Declares the factory method; may provide a default implementation |
| **ConcreteCreator** | Overrides the factory method to return a specific ConcreteProduct |

---

## C++ Implementation

### This Project's Example

```cpp
// Product interface — Input.hpp
class Input {
public:
    virtual bool Up()    = 0;
    virtual bool Down()  = 0;
    virtual bool Left()  = 0;
    virtual bool Right() = 0;
    virtual ~Input()     = default;
};

// Factory — InputFactory.hpp
class InputFactory {
public:
    static std::unique_ptr<Input> Create(const std::string& device) {
        if (device == "keyboard")       return std::make_unique<Keyboard>();
        if (device == "gamecontroller") return std::make_unique<GameController>();
        if (device == "null")           return std::make_unique<NullInput>();
        throw std::invalid_argument{"InputFactory: unknown device '" + device + "'"};
    }
};

// Client — main.cpp never calls `new Keyboard` directly
int main() {
    auto input = InputFactory::Create("keyboard");
    Game(*input);
}
```

Adding a new device (e.g. `"joystick"`) means:

1. Create `Joystick.hpp` / `Joystick.cpp`
2. Add one `if` branch to `InputFactory::Create()`
3. `main.cpp`, `Game()`, and every other call site stay **unchanged**

---

### Full GoF Creator Hierarchy

For cases where you want subclassable factories:

```cpp
#include <iostream>
#include <memory>

// Product interface
class Input {
public:
    virtual ~Input() = default;
    virtual bool Up() = 0;
};

// Concrete Products
class Keyboard : public Input {
public:
    bool Up() override { return true; }
};

class GameController : public Input {
public:
    bool Up() override { return false; }
};

// Creator — declares the factory method
class InputCreator {
public:
    virtual ~InputCreator() = default;

    // Factory method — subclasses override this
    virtual std::unique_ptr<Input> createInput() const = 0;

    // Core operation that uses the product
    void run() {
        auto input = createInput();
        std::cout << (input->Up() ? "Up pressed" : "No input") << '\n';
    }
};

// Concrete Creator A
class KeyboardCreator : public InputCreator {
public:
    std::unique_ptr<Input> createInput() const override {
        return std::make_unique<Keyboard>();
    }
};

// Concrete Creator B
class GameControllerCreator : public InputCreator {
public:
    std::unique_ptr<Input> createInput() const override {
        return std::make_unique<GameController>();
    }
};

// Client only knows the Creator interface
void launch(const InputCreator& creator) {
    creator.run();
}

int main() {
    KeyboardCreator        kc;
    GameControllerCreator  gc;

    launch(kc); // Up pressed
    launch(gc); // No input
}
```

---

## Simple Factory vs Factory Method vs Abstract Factory

```
Simple Factory (this project's InputFactory)
  └─ One static method, one place to change.
     Not strictly a GoF pattern, but the most common real-world form.
     Use when you have one creator and no need to subclass the factory.

Factory Method (full GoF)
  └─ Abstract Creator + ConcreteCreator subclasses.
     Use when the factory itself needs to vary — e.g. a framework
     that lets users plug in their own creators.

Abstract Factory
  └─ Creates families of related products.
     Use when you need to ensure a set of products work together
     (e.g. WindowsButton + WindowsCheckbox vs MacButton + MacCheckbox).
```

---

## Factory Method vs Direct Construction

```cpp
// ❌ Bad — client is tightly coupled to concrete types
int main() {
    Keyboard k;
    Game(k);
    // To switch devices you must edit main() every time
    // To test you must compile in a real Keyboard
}

// ✅ Good — client depends only on the Input interface
int main() {
    auto input = InputFactory::Create("keyboard");
    Game(*input);
    // Swap device: change one string, recompile nothing else
    // Test: InputFactory::Create("null") gives a safe NullInput
}
```

---

## Combining With Other Patterns

```cpp
// Factory Method + Strategy:
// Factory creates the right Strategy (Input), Context (Game) uses it.
auto input = InputFactory::Create("gamecontroller"); // Factory Method
Game(*input);                                        // Strategy

// Factory Method + Null Object:
// Factory can return a NullInput when no device exists,
// so callers never receive nullptr.
auto input = InputFactory::Create("null"); // safe do-nothing Input
Game(*input);                              // Game() needs no null-check
```

---

## Factory Method vs Similar Patterns

| Pattern              | Intent                                                     |
| -------------------- | ---------------------------------------------------------- |
| **Factory Method**   | Creates _one product_; subclasses decide the concrete type |
| **Abstract Factory** | Creates _families_ of related products                     |
| **Builder**          | Constructs a complex object step by step                   |
| **Prototype**        | Creates objects by cloning an existing instance            |
| **Singleton**        | Ensures only one instance exists                           |

> Key distinction: **Factory Method** is about _which class_ to instantiate. **Builder** is about _how_ to construct a complex object incrementally.

---

## Pros and Cons

### Pros

- Follows the **Open/Closed Principle** — add new products without touching existing code
- Follows the **Single Responsibility Principle** — creation logic lives in one place
- Decouples the client from concrete product classes
- Makes swapping products trivial (change one string / one creator)
- Simplifies testing — inject a `NullInput` or mock via the factory

### Cons

- Can introduce many small subclasses if the full Creator hierarchy is used
- The simple factory variant is not strictly extensible without editing the factory itself
- Overkill if you only ever create one type of product

---

## Common Mistakes

```cpp
// ❌ Bad — returning raw pointers; ownership is unclear
Input* InputFactory::Create(const std::string& device) {
    if (device == "keyboard") return new Keyboard(); // who deletes this?
}

// ✅ Good — return unique_ptr; ownership is explicit and automatic
std::unique_ptr<Input> InputFactory::Create(const std::string& device) {
    if (device == "keyboard") return std::make_unique<Keyboard>();
}
```

```cpp
// ❌ Bad — factory does too much (configuration, logging, caching)
std::unique_ptr<Input> InputFactory::Create(const std::string& device) {
    log("Creating " + device);          // not factory's job
    auto input = std::make_unique<Keyboard>();
    input->loadConfig("keyboard.ini");  // not factory's job
    return input;
}

// ✅ Good — factory only decides which type to construct
std::unique_ptr<Input> InputFactory::Create(const std::string& device) {
    if (device == "keyboard") return std::make_unique<Keyboard>();
}
```

---

## Quick Reference

```
Problem:  main() (or any client) is calling `new ConcreteClass()`
          and you want to stop that dependency spreading everywhere.

Solution: Create InputFactory::Create(name) that owns all the
          `new`/make_unique calls. Client asks by name, receives
          a Product interface pointer, never sees the concrete type.

Rule of thumb:
  - One factory, no subclassing needed?  → Simple Factory (static method)
  - Factory itself needs to vary?        → Full Factory Method (Creator hierarchy)
  - Creating families of products?       → Abstract Factory
  - Object is expensive to construct?    → Consider Prototype (clone) or Builder
```

---

## See Also

- `docs/Adapter.md` — wrapping a concrete class behind an interface (structural)
- `docs/Strategy.md` — the Input interface this factory produces
- `docs/NullObject.md` _(planned)_ — the "null" product the factory can return safely
