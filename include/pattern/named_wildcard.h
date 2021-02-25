#ifndef PATTERN_NAMED_WILDCARD_H
#define PATTERN_NAMED_WILDCARD_H

#include "pattern/wildcard.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

class NamedWildcard final : public Wildcard {
public: // Aliases
    using Ptr = std::unique_ptr<NamedWildcard>;
    using Vec = std::vector<Ptr>;

public: // Methods
    NamedWildcard(const yy::location &loc, std::string name);
    ~NamedWildcard() override = default;
    [[nodiscard]] const std::string &getNodeName() const override;
    const std::string *              getName() override;

private: // Fields
    std::string name_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_NAMED_WILDCARD_H
