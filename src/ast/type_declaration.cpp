#include "ast/type_declaration.h"

#include "type/type.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function, std::ostream, std::string, std::unique_ptr;

namespace AST {

TypeDeclaration::TypeDeclaration(
    const yy::location &          loc,
    const yy::location &          varLoc,
    string                        variable,
    unique_ptr<TypeChecker::Type> declType)
    : Declaration(loc, varLoc, move(variable))
    , declType_{move(declType)} {}

TypeDeclaration::~TypeDeclaration() = default;

void
TypeDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "type declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("type", declType_);
}

const TypeChecker::Type &
TypeDeclaration::getDeclType() const {
    return *declType_;
}

const string &
TypeDeclaration::getNodeName() const {
    const static string name = "Type Decl";
    return name;
}

TypeChecker::Type &
TypeDeclaration::getTypeImpl(TypeChecker::Context &ctx) {
    declType_->verify(ctx);
    assignType(ctx, *declType_, false);
    return *declType_;
}

void
TypeDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    declType_->walk(fn);
}

} // namespace AST
