#ifndef PATTERN_CONSTRAINT_H
#define PATTERN_CONSTRAINT_H

#include "pattern/pattern.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

class Constraint : public Pattern {
public: // Methods
    explicit Constraint(const yy::location &loc);
    ~Constraint() override;
};

} // namespace Pattern

#endif // PATTERN_CONSTRAINT_H
