#include "ast/func_impl.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

FuncImpl::FuncImpl(
    const yy::location &  loc,
    string                variable,
    Pattern::Pattern::Vec args,
    Block::Ptr            body,
    optional<Type::Ptr>   retType)
    : FuncDeclaration(loc, move(variable), move(args), move(retType))
    , body_{move(body)} {}

void
FuncImpl::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "function impl");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("args", getArgs());
    obj.printKeyValue("return type", getRetType());
    obj.printKeyValue("body", body_);
}

void
FuncImpl::walk(const Func &fn) const {
    FuncDeclaration::walk(fn);
    body_->walk(fn);
}

const string &
FuncImpl::getTypeName() const {
    const static string name = "Func Impl";
    return name;
}

} // namespace AST
