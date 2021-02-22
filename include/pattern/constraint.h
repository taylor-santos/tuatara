#ifndef PATTERN_CONSTRAINT_H
#define PATTERN_CONSTRAINT_H

#include "pattern/pattern.h"

namespace Pattern {

class Constraint : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<Constraint>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Constraint(const yy::location &loc);
    ~Constraint() override = default;
};

} // namespace Pattern

#endif // PATTERN_CONSTRAINT_H
