#include "ast/type_declaration.h"

#include "type/type_exception.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

TypeDeclaration::TypeDeclaration(
    const yy::location &loc,
    const yy::location &varLoc,
    string              variable,
    Type::Ptr           declType)
    : Declaration(loc, varLoc, move(variable))
    , declType_{move(declType)} {}

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

Type &
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
