#include "ast/declaration.h"

#include <sstream>

#include "type/type_exception.h"

using namespace std;

namespace AST {

Declaration::Declaration(const yy::location &loc, string variable)
    : Expression(loc)
    , variable_{move(variable)} {}

const string &
Declaration::getVariable() const {
    return variable_;
}

void
Declaration::assignType(TypeChecker::Context &ctx, TypeChecker::Type &type, bool init) const {
    auto symbol = ctx.getSymbol(getVariable());
    if (symbol) {
        if (type <= symbol->type) {
            symbol->initialized = init;
            return;
        }
        stringstream ss;
        ss << "note: \"" << getVariable() << "\" previously defined as \"";
        symbol->type.pretty(ss);
        ss << "\" here:";
        throw TypeChecker::TypeException(
            {{"error: redefining variable \"" + getVariable() + "\"", getLoc()},
             {ss.str(), (*symbol).type.getLoc()}});
    }
    ctx.setSymbol({getVariable(), type, init});
}

} // namespace AST
