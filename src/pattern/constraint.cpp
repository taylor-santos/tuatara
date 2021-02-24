#include "pattern/constraint.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace Pattern {

Constraint::Constraint(const yy::location &loc)
    : Pattern(loc) {}

} // namespace Pattern
