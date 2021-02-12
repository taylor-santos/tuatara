#ifndef PATTERN_WILDCARD_H
#define PATTERN_WILDCARD_H

#include "pattern/pattern.h"

namespace Pattern {

class Wildcard : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<Wildcard>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Wildcard(const yy::location &loc);
    ~Wildcard() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_WILDCARD_H
