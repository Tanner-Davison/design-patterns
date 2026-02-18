# Adapter Pattern

## What Is It?

The Adapter pattern is a **structural design pattern** that allows two incompatible interfaces to work together. It acts as a wrapper — you have an existing class with an interface you can't (or don't want to) change, and you need it to work with code that expects a different interface. The Adapter sits in between and translates calls from one interface to the other.

Think of it like a power outlet adapter: your laptop plug doesn't fit the wall socket, so you use an adapter that converts one shape to the other without changing either.

---

## When To Use It

- You want to use an existing class but its interface doesn't match what you need
- You're integrating a third-party library or legacy code into a new system
- You want to create a reusable class that cooperates with classes that have incompatible interfaces
- You need to adapt several existing subclasses but don't want to subclass each one just to fix their interfaces

---

## Structure

There are two forms of the Adapter pattern in C++:

### Object Adapter (preferred)
Uses **composition** — holds an instance of the adaptee internally.

```
┌─────────────┐        ┌─────────────────┐        ┌─────────────┐
│   Client    │──────▶ │     Adapter     │──────▶ │  Adaptee    │
│             │        │ +request()      │        │ +specificReq│
└─────────────┘        │  (translates)   │        └─────────────┘
                       └─────────────────┘
       uses Target interface          wraps Adaptee
```

### Class Adapter
Uses **multiple inheritance** — inherits from both the target interface and the adaptee.

```
┌─────────────┐        ┌──────────────────────────┐
│   Client    │──────▶ │   Adapter                │
└─────────────┘        │ inherits Target + Adaptee│
                       └──────────────────────────┘
```

> In modern C++, **Object Adapter is preferred** — it's more flexible and avoids the pitfalls of multiple inheritance.

---

## Participants

| Participant | Role |
|-------------|------|
| **Target** | The interface the client expects to use |
| **Adaptee** | The existing class with an incompatible interface |
| **Adapter** | Wraps the Adaptee and implements the Target interface |
| **Client** | Interacts only with the Target interface |

---

## C++ Implementation

### Object Adapter

```cpp
#include <iostream>
#include <string>
#include <memory>

// Target interface — what the client expects
class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& message) = 0;
};

// Adaptee — existing class with incompatible interface
class LegacyLogger {
public:
    void writeLog(const std::string& msg, int severity) {
        std::cout << "[SEV:" << severity << "] " << msg << '\n';
    }
};

// Adapter — wraps LegacyLogger, implements Logger
class LoggerAdapter : public Logger {
public:
    explicit LoggerAdapter(std::shared_ptr<LegacyLogger> legacy)
        : m_legacy(std::move(legacy)) {}

    void log(const std::string& message) override {
        // Translate the call — default severity of 1
        m_legacy->writeLog(message, 1);
    }

private:
    std::shared_ptr<LegacyLogger> m_legacy;
};

// Client — only knows about Logger
void sendAlert(Logger& logger, const std::string& msg) {
    logger.log(msg);
}

int main() {
    auto legacy  = std::make_shared<LegacyLogger>();
    LoggerAdapter adapter(legacy);

    sendAlert(adapter, "Disk usage above 90%");
    // Output: [SEV:1] Disk usage above 90%
}
```

---

### Class Adapter (multiple inheritance)

```cpp
#include <iostream>
#include <string>

class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& message) = 0;
};

class LegacyLogger {
public:
    void writeLog(const std::string& msg, int severity) {
        std::cout << "[SEV:" << severity << "] " << msg << '\n';
    }
};

// Inherits interface from Logger, implementation access from LegacyLogger
class LoggerAdapter : public Logger, private LegacyLogger {
public:
    void log(const std::string& message) override {
        writeLog(message, 1);
    }
};

int main() {
    LoggerAdapter adapter;
    adapter.log("Disk usage above 90%");
    // Output: [SEV:1] Disk usage above 90%
}
```

---

## Real-World C++ Example — Adapting a Third-Party JSON Library

```cpp
#include <iostream>
#include <string>
#include <memory>

// Your app's serializer interface
class Serializer {
public:
    virtual ~Serializer() = default;
    virtual std::string serialize(const std::string& key, const std::string& value) = 0;
};

// Third-party library you don't control
class ThirdPartyJson {
public:
    std::string toJson(const std::string& k, const std::string& v) {
        return "{\"" + k + "\": \"" + v + "\"}";
    }
};

// Adapter bridges your interface with the third-party lib
class JsonSerializerAdapter : public Serializer {
public:
    explicit JsonSerializerAdapter(std::shared_ptr<ThirdPartyJson> lib)
        : m_lib(std::move(lib)) {}

    std::string serialize(const std::string& key, const std::string& value) override {
        return m_lib->toJson(key, value);
    }

private:
    std::shared_ptr<ThirdPartyJson> m_lib;
};

void saveData(Serializer& s) {
    std::cout << s.serialize("username", "tanner") << '\n';
}

int main() {
    auto lib = std::make_shared<ThirdPartyJson>();
    JsonSerializerAdapter adapter(lib);
    saveData(adapter);
    // Output: {"username": "tanner"}
}
```

---

## Object Adapter vs Class Adapter

| | Object Adapter | Class Adapter |
|---|---|---|
| Mechanism | Composition | Multiple inheritance |
| Can adapt subclasses? | Yes | No (binds to one concrete class) |
| Can override Adaptee behavior? | No (without subclassing) | Yes |
| Flexibility | Higher | Lower |
| Preferred in modern C++? | ✅ Yes | ⚠️ Rarely |

---

## Adapter vs Similar Patterns

| Pattern | Intent |
|---------|--------|
| **Adapter** | Makes incompatible interfaces compatible (after the fact) |
| **Facade** | Simplifies a complex subsystem with a new simplified interface |
| **Decorator** | Adds behavior to an object without changing its interface |
| **Proxy** | Controls access to an object using the same interface |
| **Bridge** | Separates abstraction from implementation (designed upfront) |

> Key distinction: **Adapter** changes the interface. **Decorator** keeps it. **Facade** hides it.

---

## Pros and Cons

### Pros
- Follows the **Single Responsibility Principle** — separation of interface conversion from business logic
- Follows the **Open/Closed Principle** — add new adapters without changing existing code
- Makes legacy/third-party code reusable without modification
- Client code stays clean — it only ever sees the target interface

### Cons
- Adds an extra layer of indirection (slight complexity overhead)
- Sometimes it's cleaner to just refactor the original class if you own it
- Class Adapter form ties you to a specific concrete Adaptee class

---

## Common Mistakes

```cpp
// ❌ Bad — directly modifying the adaptee to fit your needs
// Violates Open/Closed, breaks existing users of LegacyLogger
class LegacyLogger {
    void log(const std::string& msg) { ... } // renamed to match target
};

// ✅ Good — wrap it, don't touch it
class LoggerAdapter : public Logger {
    void log(const std::string& msg) override {
        m_legacy->writeLog(msg, 1); // translate here
    }
};
```

```cpp
// ❌ Bad — adapter doing too much business logic
void log(const std::string& message) override {
    auto formatted = formatMessage(message);   // not adapter's job
    auto filtered  = filterSensitive(formatted); // not adapter's job
    m_legacy->writeLog(filtered, 1);
}

// ✅ Good — adapter only translates the interface
void log(const std::string& message) override {
    m_legacy->writeLog(message, 1);
}
```

---

## Quick Reference

```
Problem:  I have ClassA and ClassB. Client expects ClassA's interface
          but I need to use ClassB's functionality.

Solution: Create Adapter : public ClassA {
              ClassB* m_b;
              void methodA() override { m_b->methodB(); }
          }

Rule of thumb:
  - Own the code?        → Refactor instead
  - Third-party/legacy?  → Use Adapter
  - Need extra behavior? → Combine with Decorator
  - Too many adapters?   → Consider a Facade on top
```

---

## See Also

- `docs/Facade.md` — simplifying a complex subsystem
- `docs/Decorator.md` — adding behavior without changing interfaces
- `docs/Bridge.md` — decoupling abstraction from implementation
