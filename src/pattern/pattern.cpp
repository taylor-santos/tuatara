#include "pattern/pattern.h"

#include "type/type_context.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

Pattern::Pattern(const yy::location &loc)
    : AST::Node(loc) {}

const std::string *
Pattern::getName() {
    return nullptr;
}

TypeChecker::Type &
Pattern::getType(TypeChecker::Context &ctx) {
    auto &type = getTypeImpl(ctx);
    if (auto *name = getName()) {
        ctx.setSymbol({*name, type, true});
    }
    return type;
}

} // namespace Pattern
