#ifndef TYPE_CONTEXT_H
#define TYPE_CONTEXT_H

#include <optional>
#include <string>
#include <unordered_map>

namespace TypeChecker {

class Type;
class Func;

class Context {
public: // Classes
    struct Symbol {
        std::string        name;
        TypeChecker::Type &type;
        bool               initialized;
    };

public: // Methods
    Context();
    Context(const Context &other)               = default;
    auto                                operator=(const Context &) = delete;
    [[nodiscard]] std::optional<Symbol> getSymbol(const std::string &name) const;
    void                                setSymbol(const Symbol &symbol);
    Func *                              getFuncSignature() const;
    void                                setFuncSignature(Func *funcSignature);

private: // Fields
    std::unordered_map<std::string, Symbol> symbols_;
    // Points to the context signature if inside the body of a function, nullptr otherwise.
    Func *funcSignature_ = nullptr;
};

} // namespace TypeChecker

#endif // TYPE_CONTEXT_H
