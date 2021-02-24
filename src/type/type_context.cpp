#include "type/type_context.h"

#include "type/class.h"
#include "type/func.h"
#include "type/object.h"

using namespace std;

namespace TypeChecker {

unique_ptr<Class>
makeClass(const std::string &name) {
    yy::location loc{nullptr, 0, 0};
    auto         cl = make_unique<Class>(loc, name);
    for (const auto &op : {"+"}) {
        auto fn =
            make_unique<Func>(loc, make_unique<Object>(loc, name), make_unique<Object>(loc, name));
        (void)cl->addField(op, move(fn));
    }
    return cl;
}

Context::Context() {
    for (const auto &name : {"int", "float", "string", "bool"}) {
        const auto &[it, _] = builtins_.emplace(name, makeClass(name));
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
