#ifndef PATTERN_PATTERN_H
#define PATTERN_PATTERN_H

#include "ast.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

class Pattern : public AST::Node {
public: // Aliases
    using Ptr = std::unique_ptr<Pattern>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Pattern(const yy::location &loc);
};

} // namespace Pattern

#endif // PATTERN_PATTERN_H
