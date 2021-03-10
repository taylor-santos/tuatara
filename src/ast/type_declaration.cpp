#include "ast/type_declaration.h"

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

TypeDeclaration::TypeDeclaration(
    const yy::location &          loc,
    const yy::location &          varLoc,
    string                        variable,
    shared_ptr<TypeChecker::Type> declType)
    : Declaration(loc, varLoc, move(variable))
    , declType_{move(declType)} {
    assert(declType_);
}

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

shared_ptr<TypeChecker::Type>
TypeDeclaration::getDeclTypeImpl(TypeChecker::Context &ctx) {
    declType_ = declType_->verify(ctx);
    return declType_;
}

void
TypeDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    declType_->walk(fn);
}

} // namespace AST
