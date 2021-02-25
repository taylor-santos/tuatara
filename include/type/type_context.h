#ifndef TYPE_CONTEXT_H
#define TYPE_CONTEXT_H

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

namespace TypeChecker {

class Type;
class Func;
class Class;

class Context {
public: // Classes
    struct Symbol {
        std::string        name;
        TypeChecker::Type &type;
        bool               initialized;
    };

public: // Methods
    Context();
    Context(const Context &other) = default;
    ~Context();
    auto                                operator=(const Context &) = delete;
    [[nodiscard]] std::optional<Symbol> getSymbol(const std::string &name) const;
    void                                setSymbol(const Symbol &symbol);
    Func *                              getFuncSignature() const;
    void                                setFuncSignature(Func *funcSignature);
    void                                addClass(const std::string &name, Class &cl);
    [[nodiscard]] Class *               getClass(const std::string &name) const;
    const std::unordered_map<std::string, std::reference_wrapper<Class>> &getClasses() const;

private: // Fields
    std::unordered_map<std::string, Symbol>                        symbols_;
    std::unordered_map<std::string, std::reference_wrapper<Class>> classes_;
    // Points to the context signature if inside the body of a function, nullptr otherwise.
    Func *funcSignature_ = nullptr;
};

} // namespace TypeChecker

#endif // TYPE_CONTEXT_H
