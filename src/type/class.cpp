#include "type/class.h"

#include <utility>

#include "json.h"

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

void
Class::verifyImpl(Context &) {}

void
Class::pretty(ostream &out, bool) const {
    out << "<class>";
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
