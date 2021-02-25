#include "type/type_context.h"

#include "type/class.h"
#include "type/func.h"
#include "type/object.h"
#include "type/product.h"
#include "type/unit.h"

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

static unordered_map<string, Type::Ptr>
generateBuiltins() noexcept {
    static auto numerics = {"int", "float"};
    static auto logics   = {"bool"};
    static auto others   = {"string"};

    /*
    static auto prefixes  = {"-", "++", "--"};
    static auto postfixes = {
        "-",
        "++",
        "-- ",
    };
     */
    static auto infixes = {
        "+",
        "-",
        "*",
        "/",
        "+=",
        "-=",
        "*=",
        "/=",
        "=",
        "+",
    };

    static yy::location              loc{nullptr, 0, 0};
    unordered_map<string, Type::Ptr> builtins;
    for (const auto &name : numerics) {
        auto  clptr = makeClass(name);
        auto &cl    = *clptr;
        builtins.emplace(name, move(clptr));
        /*
        for (const auto &op : prefixes) {
            builtins.emplace(
                op,
                make_unique<Func>(
                    loc,
                    make_unique<Object>(loc, name),
                    make_unique<Object>(loc, name)));
        }
         */
        for (const auto &op : infixes) {
            (void)cl.addField(
                op,
                make_unique<Func>(
                    loc,
                    make_unique<Object>(loc, name),
                    make_unique<Object>(loc, name)));
        }
    }
    for (const auto &name : logics) {
        builtins.emplace(name, makeClass(name));
    }
    for (const auto &name : others) {
        builtins.emplace(name, makeClass(name));
    }
    return builtins;
}

static unordered_map<string, Type::Ptr> BUILTINS = generateBuiltins();

Context::Context() {
    for (const auto &[name, type] : BUILTINS) {
        symbols_.emplace(name, Symbol{name, *type, true});
    }
    for (const auto &[_, symbol] : symbols_) {
        symbol.type.verify(*this);
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

Func *
Context::getFuncSignature() const {
    return funcSignature_;
}
void
Context::setFuncSignature(Func *funcSignature) {
    funcSignature_ = funcSignature;
}

} // namespace TypeChecker
