#ifndef TYPE_CONTEXT_H
#define TYPE_CONTEXT_H

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace TypeChecker {

class Type;
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
    [[nodiscard]] std::optional<Symbol> getSymbol(const std::string &name) const;
    void                                setSymbol(const Symbol &symbol);

private: // Fields
    std::unordered_map<std::string, Symbol> symbols_;
};

} // namespace TypeChecker

#endif // TYPE_CONTEXT_H
