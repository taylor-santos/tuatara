#include "pattern/pattern.h"

namespace Pattern {

Pattern::Pattern(const yy::location &loc)
    : AST::Node(loc) {}

} // namespace Pattern