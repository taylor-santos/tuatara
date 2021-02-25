#include "ast/declaration.h"

#include <sstream>

#include "type/type.h"
#include "type/type_context.h"
#include "type/type_exception.h"

using std::pair, std::string, std::stringstream, std::vector;

namespace AST {

Declaration::Declaration(const yy::location &loc, const yy::location &varLoc, string variable)
    : Expression(loc)
    , varLoc_{varLoc}
    , variable_{move(variable)} {}

Declaration::~Declaration() = default;

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
        vector<pair<string, yy::location>> msgs;
        msgs.emplace_back("error: redefining variable \"" + getVariable() + "\"", varLoc_);
        stringstream ss;
        ss << "note: \"" << getVariable() << "\" previously defined as \"";
        symbol->type.pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), symbol->type.getLoc());
        throw TypeChecker::TypeException(msgs);
    }
    ctx.setSymbol({getVariable(), type, init});
}

} // namespace AST
