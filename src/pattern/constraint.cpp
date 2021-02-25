#include "pattern/constraint.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

Constraint::Constraint(const yy::location &loc)
    : Pattern(loc) {}

Constraint::~Constraint() = default;

} // namespace Pattern
