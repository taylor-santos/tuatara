#include <optional>
#include <unordered_map>

#include "type/class.h"
#include "type/type.h"

namespace TypeChecker {

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
    std::unordered_map<std::string, Symbol>                  symbols_;
    std::unordered_map<std::string, TypeChecker::Class::Ptr> builtins_;
    const static char *                                      builtinNames_[];
};

} // namespace TypeChecker
