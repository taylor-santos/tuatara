#include "type/class.h"

#include <algorithm>

#include "type/func.h"
#include "type/object.h"
#include "type/type_context.h"

#include "json.h"

using std::all_of, std::make_unique, std::ostream, std::pair, std::string, std::unique_ptr,
    std::unordered_map, std::vector;

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
    for (const auto &[name, field] : fields_) {
        field->verify(ctx);
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
            auto *otherField    = other.getField(name);
            return otherField && otherField->isSubtype(*field, ctx);
        });
        bool fieldsAllSub   = all_of(other.fields_.begin(), other.fields_.end(), [&](auto &it) {
            auto &[name, otherField] = it;
            auto *field              = getField(name);
            return field && field->isSubtype(*otherField, ctx);
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

Type *
Class::getField(const string &name) const {
    auto it = fields_.find(name);
    if (it == fields_.end()) {
        return nullptr;
    }
    return it->second.get();
}

bool
Class::addField(const string &name, unique_ptr<Type> type) {
    auto prev = fields_.find(name);
    if (prev != fields_.end()) {
        return true;
    }
    fields_.emplace(name, move(type));
    return false;
}

bool
Class::isSubtype(const Type &other, Context &ctx) const {
    return other.isSupertype(*this, ctx);
}

bool
Class::isSupertype(const Type &other, Context &ctx) const {
    return other.isSubtype(*this, ctx);
}

bool
Class::isSupertype(const Class &other, Context &) const {
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

unordered_map<string, unique_ptr<Class>>
Class::generateBuiltins() {
    static auto numerics = {"int", "float"};
    static auto logics   = {"bool"};
    static auto others   = {"string"};

    static yy::location                      loc{nullptr, 0, 0};
    unordered_map<string, unique_ptr<Class>> builtins;
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
            "+",
        };
        auto  clptr = make_unique<Class>(loc, name);
        auto &cl    = *clptr;
        cl.setVerifyState(Type::VerifyState::VERIFIED);
        builtins.emplace(name, move(clptr));
        for (const auto &op : infixes) {
            (void)cl.addField(
                op,
                make_unique<Func>(
                    loc,
                    make_unique<Object>(loc, name),
                    make_unique<Object>(loc, name)));
        }
    }
    for (const auto &name : logics) {
        auto  clptr = make_unique<Class>(loc, name);
        auto &cl    = *clptr;
        cl.setVerifyState(Type::VerifyState::VERIFIED);
        builtins.emplace(name, move(clptr));
    }
    for (const auto &name : others) {
        auto  clptr = make_unique<Class>(loc, name);
        auto &cl    = *clptr;
        cl.setVerifyState(Type::VerifyState::VERIFIED);
        builtins.emplace(name, move(clptr));
    }
    builtins["int"]->addSubType(&*builtins["bool"]);
    return builtins;
}

} // namespace TypeChecker
