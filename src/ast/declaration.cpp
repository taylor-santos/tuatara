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
        if (type.isSubtype(symbol->type, ctx)) {
            symbol->initialized = init;
            return;
        }
        vector<pair<string, yy::location>> msgs;
        stringstream                       ss;
        ss << "error: redefining variable \"" << getVariable() << "\" to have type \"";
        type.pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), varLoc_);
        stringstream ss2;
        ss2 << "note: \"" << getVariable() << "\" defined to have type \"";
        symbol->type.pretty(ss2);
        ss2 << "\"";
        msgs.emplace_back(ss2.str(), symbol->type.getLoc());
        throw TypeChecker::TypeException(msgs);
    }
    ctx.setSymbol({getVariable(), type, init});
}

} // namespace AST
