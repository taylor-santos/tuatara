#include "type/type_context.h"

#include "type/class.h"

using namespace std;

namespace TypeChecker {

const char *Context::builtinNames_[] = {"int", "float", "string", "bool"};

Context::Context() {
    for (const auto &name : builtinNames_) {
        const auto &[it, _] = builtins_.emplace(name, make_unique<Class>(yy::location{}, name));
        symbols_.emplace(name, Symbol{name, *it->second, true});
    }
}

optional<Context::Symbol>
Context::getSymbol(const std::string &name) const {
    auto it = symbols_.find(name);
    if (it == symbols_.end()) {
        return std::nullopt;
    }
    return it->second;
}

void
Context::setSymbol(const Context::Symbol &symbol) {
    symbols_.emplace(symbol.name, symbol);
}

} // namespace TypeChecker
