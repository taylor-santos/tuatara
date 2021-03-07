#include "type/array.h"

#include <cassert>
#include <sstream>

#include "ast/expression.h"

#include "type/class.h"
#include "type/maybe.h"
#include "type/object.h"
#include "type/product.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::dynamic_pointer_cast;
using std::function;
using std::make_shared;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace TypeChecker {

Array::Array(yy::location loc, shared_ptr<Type> type)
    : Type(loc)
    , type_{move(type)} {
    assert(type_);
}

Array::~Array() = default;

void
Array::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "array");
    obj.printKeyValue("type", type_);
}

void
Array::walk(const function<void(const Node &)> &fn) const {
    Type::walk(fn);
    type_->walk(fn);
}

const string &
Array::getNodeName() const {
    const static string name = "Array Type";
    return name;
}

void
Array::verifyImpl(Context &ctx) {
    type_ = TypeChecker::Type::verify(type_, ctx);
}

void
Array::pretty(ostream &out, bool) const {
    type_->pretty(out, true);
    out << "[]";
}

bool
Array::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Array::isSuperImpl(const class Array &other, const Context &ctx) const {
    return other.type_->isSubtype(*type_, ctx);
}

shared_ptr<Type>
Array::simplify(Context &ctx) {
    type_ = type_->simplify(ctx);
    return shared_from_this();
}

shared_ptr<Type>
Array::indexAsArray(AST::Expression &arg, const AST::Expression &index, Context &ctx) {
    auto argType  = arg.getType(ctx);
    auto argAsObj = dynamic_pointer_cast<TypeChecker::Object>(argType);
    if (argAsObj && argAsObj->getClass().getClassName() == "int") {
        return make_shared<TypeChecker::Maybe>(getLoc(), type_);
    }
    vector<pair<string, yy::location>> msgs;
    {
        stringstream ss;
        ss << "error: cannot index into array with expression of type \"";
        argType->pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), index.getLoc());
    }
    argType->addTypeLocMessage(msgs);
    throw TypeChecker::TypeException(msgs);
}

} // namespace TypeChecker
