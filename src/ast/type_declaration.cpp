#include "ast/type_declaration.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

TypeDeclaration::TypeDeclaration(const yy::location &loc, string variable, Type::Ptr type)
    : Declaration(loc, move(variable))
    , type_{move(type)} {}

void
TypeDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "type declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("type", type_);
}

const TypeChecker::Type &
TypeDeclaration::getType() const {
    return *type_;
}

const string &
TypeDeclaration::getTypeName() const {
    const static string name = "Type Decl";
    return name;
}

void
TypeDeclaration::walk(const Func &fn) const {
    Declaration::walk(fn);
    type_->walk(fn);
}

} // namespace AST
