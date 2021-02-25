#include "type/type_context.h"

#include "type/class.h"
#include "type/func.h"
#include "type/object.h"
#include "type/product.h"
#include "type/unit.h"

using std::make_unique, std::nullopt, std::optional, std::string, std::unique_ptr,
    std::unordered_map;

namespace TypeChecker {

static unordered_map<string, unique_ptr<Class>> BUILTINS = Class::generateBuiltins();

Context::~Context() = default;

Context::Context() {
    for (const auto &[name, type] : BUILTINS) {
        addClass(name, *type);
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

void
Context::addClass(const std::string &name, Class &cl) {
    classes_.emplace(name, cl);
}

Class *
Context::getClass(const string &name) const {
    auto it = classes_.find(name);
    if (it == classes_.end()) {
        return nullptr;
    }
    return &it->second.get();
}

const std::unordered_map<std::string, std::reference_wrapper<Class>> &
Context::getClasses() const {
    return classes_;
}

} // namespace TypeChecker
