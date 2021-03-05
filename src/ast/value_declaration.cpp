#include "ast/value_declaration.h"

#include <cassert>

#include "type/type.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace AST {

ValueDeclaration::ValueDeclaration(
    const yy::location &   loc,
    const yy::location &   varLoc,
    string                 variable,
    unique_ptr<Expression> value)
    : Declaration(loc, varLoc, move(variable))
    , value_{move(value)} {
    assert(value_);
}

ValueDeclaration::~ValueDeclaration() = default;

void
ValueDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "value declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("value", value_);
}

const Expression &
ValueDeclaration::getValue() const {
    return *value_;
}

void
ValueDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    value_->walk(fn);
}

const string &
ValueDeclaration::getNodeName() const {
    const static string name = "Value Decl";
    return name;
}

shared_ptr<TypeChecker::Type>
ValueDeclaration::getDeclTypeImpl(TypeChecker::Context &ctx) {
    auto type = value_->getType(ctx);
    type->setInitialized(true);
    return type;
}

} // namespace AST
