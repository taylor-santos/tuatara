#include "type/class.h"

#include <algorithm>

#include "type/func.h"
#include "type/object.h"
#include "type/type_context.h"

#include "json.h"

using std::all_of;
using std::logic_error;
using std::make_shared;
using std::make_unique;
using std::nullopt;
using std::optional;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace TypeChecker {

class Context;

Class::Class(yy::location loc, string name)
    : Type(loc)
    , name_{move(name)} {
    addSuperType(this);
    addSubType(this);
}

Class::~Class() = default;

void
Class::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "class");
}

const string &
Class::getNodeName() const {
    const static string name = "Class Type";
    return name;
}

const string &
Class::getClassName() const {
    return name_;
}

void
Class::verifyImpl(Context &ctx) {
    for (auto &[name, field] : fields_) {
        field = field->verify(ctx);
    }
    // TODO: check if this is necessary
    setVerifyState(VerifyState::VERIFIED);
    for (const auto &[cName, cl] : ctx.getClasses()) {
        auto &other = cl.get();
        if (this == &other) {
            continue;
        }
        bool fieldsAllSuper = all_of(fields_.begin(), fields_.end(), [&](auto &it) {
            auto &[name, field] = it;
            auto optField       = other.getField(name);
            return optField && (*optField)->isSubtype(*field, ctx);
        });
        bool fieldsAllSub   = all_of(other.fields_.begin(), other.fields_.end(), [&](auto &it) {
            auto &[name, otherField] = it;
            auto optField            = getField(name);
            return optField && (*optField)->isSubtype(*otherField, ctx);
        });
        if (fieldsAllSuper) {
            addSuperType(&other);
        }
        if (fieldsAllSub) {
            addSubType(&other);
        }
    }
}

void
Class::pretty(ostream &out, bool) const {
    out << "<class>";
}

optional<shared_ptr<Type>>
Class::getField(const string &name) const {
    auto it = fields_.find(name);
    if (it == fields_.end()) {
        return nullopt;
    }
    return it->second;
}

void
Class::addField(const string &name, shared_ptr<Type> type) {
    auto prev = fields_.find(name);
    if (prev != fields_.end()) {
        throw logic_error("Class \"" + name_ + "\" already has a field \"" + name + "\"");
    }
    fields_.emplace(name, move(type));
}

bool
Class::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Class::isSuperImpl(const Class &other, const Context &) const {
    if (this == &other) {
        return true;
    }
    auto rel = classRltns_.find(&other);
    return rel != classRltns_.end() && rel->second == ClassRelation::SUPERTYPE;
}

void
Class::addSubType(Class *cl) {
    addSubTypeImpl(cl);
    cl->addSuperTypeImpl(this);
}

void
Class::addSuperType(Class *cl) {
    addSuperTypeImpl(cl);
    cl->addSubTypeImpl(this);
}

void
Class::addSubTypeImpl(Class *cl) {
    classRltns_[cl] = ClassRelation::SUBTYPE;
}

void
Class::addSuperTypeImpl(Class *cl) {
    classRltns_[cl] = ClassRelation::SUPERTYPE;
}

unordered_map<string, shared_ptr<Class>>
Class::generateBuiltins() {
    static auto numerics = {"int", "float"};
    static auto logics   = {"bool"};
    static auto others   = {"string"};

    static yy::location                      loc{nullptr, 0, 0};
    unordered_map<string, shared_ptr<Class>> builtins;
    for (const auto &name : numerics) {
        static auto infixes = {
            "+",
            "-",
            "*",
            "/",
            "+=",
            "-=",
            "*=",
            "/=",
            "=",
        };
        auto  clptr = make_shared<Class>(loc, name);
        auto &cl    = *clptr;
        cl.setVerifyState(Type::VerifyState::VERIFIED);
        builtins.emplace(name, move(clptr));
        for (const auto &op : infixes) {
            cl.addField(
                op,
                make_shared<Func>(
                    loc,
                    make_shared<Object>(loc, name),
                    make_shared<Object>(loc, name)));
        }
    }
    for (const auto &name : logics) {
        static auto infixes = {
            "||",
            "&&",
            "^",
            "||=",
            "&&=",
            "^=",
            "=",
        };
        auto  clptr = make_shared<Class>(loc, name);
        auto &cl    = *clptr;
        cl.setVerifyState(Type::VerifyState::VERIFIED);
        builtins.emplace(name, move(clptr));
        for (const auto &op : infixes) {
            cl.addField(
                op,
                make_shared<Func>(
                    loc,
                    make_shared<Object>(loc, name),
                    make_shared<Object>(loc, name)));
        }
    }
    for (const auto &name : others) {
        static auto infixes = {
            "+=",
            "=",
        };
        auto  clptr = make_shared<Class>(loc, name);
        auto &cl    = *clptr;
        cl.setVerifyState(Type::VerifyState::VERIFIED);
        builtins.emplace(name, move(clptr));
        for (const auto &op : infixes) {
            cl.addField(
                op,
                make_shared<Func>(
                    loc,
                    make_shared<Object>(loc, name),
                    make_shared<Object>(loc, name)));
        }
    }
    builtins["int"]->addSuperType(builtins["bool"].get());
    return builtins;
}

shared_ptr<Type>
Class::simplify(Context &ctx) {
    for (auto &[name, type] : fields_) {
        type = type->simplify(ctx);
    }
    return shared_from_this();
}

void
Class::updateWith(const Type &other) {
    other.updateForImpl(*this);
}

shared_ptr<Type>
Class::clone(const yy::location &) const {
    throw std::logic_error("internal compiler error: class type should never be cloned (" LOC_STR
                           ")");
}

} // namespace TypeChecker
