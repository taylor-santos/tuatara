#include "type/class.h"

#include <algorithm>

#include "json.h"

using std::all_of, std::ostream, std::string, std::unique_ptr;

namespace TypeChecker {

class Context;

Class::Class(yy::location loc, string name)
    : Type(loc)
    , name_{move(name)} {}

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
Class::isSubtype(const Type &other) const {
    return other.isSupertype(*this);
}

bool
Class::isSupertype(const Type &other) const {
    return other.isSubtype(*this);
}

bool
Class::isSupertype(const Class &other) const {
    if (this == &other) {
        return true;
    }
    // TODO: fix the infinite recursion here
    return all_of(fields_.begin(), fields_.end(), [&](auto &it) {
        auto &[name, field] = it;
        auto *otherField    = other.getField(name);
        return otherField && otherField->isSubtype(*field);
    });
}

} // namespace TypeChecker
