#include "pattern/pattern.h"

#include "type/type_context.h"

namespace yy {
class location;
} // namespace yy

using std::make_shared;
using std::shared_ptr;
using std::string;

namespace Pattern {

Pattern::Pattern(const yy::location &loc)
    : AST::Node(loc) {}

Pattern::~Pattern() = default;

const string *
Pattern::getName() {
    return nullptr;
}

shared_ptr<TypeChecker::Type>
Pattern::getType(TypeChecker::Context &ctx) {
    auto type = getTypeImpl(ctx);
    if (auto *name = getName()) {
        ctx.setSymbol(*name, type);
    }
    return type;
}

} // namespace Pattern
