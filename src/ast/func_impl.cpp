#include "ast/func_impl.h"

#include "ast/block.h"

#include "pattern/pattern.h"

#include "type/type.h"
#include "type/type_exception.h"

#include "json.h"

using std::function, std::optional, std::ostream, std::string, std::unique_ptr, std::vector;

namespace AST {

FuncImpl::FuncImpl(
    const yy::location &                    loc,
    const yy::location &                    varLoc,
    string                                  variable,
    vector<unique_ptr<Pattern::Pattern>>    args,
    unique_ptr<Block>                       body,
    optional<unique_ptr<TypeChecker::Type>> retType)
    : FuncDeclaration(loc, varLoc, move(variable), move(args), move(retType))
    , body_{move(body)} {}

FuncImpl::~FuncImpl() = default;

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
FuncImpl::walk(const function<void(const Node &)> &fn) const {
    FuncDeclaration::walk(fn);
    body_->walk(fn);
}

const string &
FuncImpl::getNodeName() const {
    const static string name = "Func Impl";
    return name;
}

TypeChecker::Type &
FuncImpl::getTypeImpl(TypeChecker::Context &ctx) {
    auto &sig = FuncDeclaration::getTypeImpl(ctx);
    (void)sig; // TODO
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

} // namespace AST
