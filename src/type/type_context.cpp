#include "type/type_context.h"

#include "type/class.h"
#include "type/func.h"
#include "type/object.h"

using namespace std;

namespace TypeChecker {

static Class::Ptr
makeClass(const string &name) {
    yy::location loc{nullptr, 0, 0};
    auto         cl = make_unique<Class>(loc, name);
    for (const auto &op : {"+"}) {
        auto fn =
            make_unique<Func>(loc, make_unique<Object>(loc, name), make_unique<Object>(loc, name));
        (void)cl->addField(op, move(fn));
    }
    return cl;
}

static unordered_map<string, Class::Ptr>
generateBuiltins() noexcept {
    unordered_map<string, Class::Ptr> builtins;
    for (const auto &name : {"int", "float", "string", "bool"}) {
        builtins.emplace(name, makeClass(name));
    }
    return builtins;
}

static unordered_map<string, Class::Ptr> BUILTINS = generateBuiltins();

Context::Context() {
    for (const auto &[name, type] : BUILTINS) {
        symbols_.emplace(name, Symbol{name, *type, true});
    }
}

optional<Context::Symbol>
Context::getSymbol(const string &name) const {
    auto it = symbols_.find(name);
    if (it == symbols_.end()) {
        return nullopt;
    }
    return it->second;
}

void
Context::setSymbol(const Context::Symbol &symbol) {
    symbols_.emplace(symbol.name, symbol);
}

} // namespace TypeChecker
