#ifndef PATTERN_TUPLE_H
#define PATTERN_TUPLE_H

#include "pattern/pattern.h"

namespace Pattern {

class Tuple final : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<Tuple>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Tuple(const yy::location &loc, Pattern::Vec patterns);
    ~Tuple() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Pattern::Vec patterns_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_TUPLE_H
