#include "pattern/pattern.h"

namespace yy {
class location;
}  // namespace yy

namespace Pattern {

Pattern::Pattern(const yy::location &loc)
    : AST::Node(loc) {}

} // namespace Pattern