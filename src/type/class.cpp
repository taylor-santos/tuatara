#include "type/class.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using namespace TypeChecker;
using namespace std;

Class::Class(yy::location loc, string name)
    : Type(loc)
    , name_{move(name)} {}

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
Class::verifyImpl(Context &) {}

void
Class::pretty(ostream &out, bool) const {
    out << "<class>";
}

Type *
Class::getField(const std::string &name) const {
    auto it = fields_.find(name);
    if (it == fields_.end()) {
        return nullptr;
    }
    return it->second.get();
}

bool
Class::addField(const string &name, Type::Ptr type) {
    auto prev = fields_.find(name);
    if (prev != fields_.end()) {
        return true;
    }
    fields_.emplace(name, move(type));
    return false;
}

bool
Class::operator<=(const Type &other) const {
    return other >= (*this);
}

bool
Class::operator>=(const Class &other) const {
    // TODO implement actual class comparison
    return this == &other;
}
