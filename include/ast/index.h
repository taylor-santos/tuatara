#ifndef AST_INDEX_H
#define AST_INDEX_H

#include "lvalue.h"

namespace AST {

class Index final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Index>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Index(const yy::location &loc, Expression::Ptr expr, Expression::Ptr index);
    ~Index() override = default;

private: // Fields
    Expression::Ptr expr;
    Expression::Ptr index;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_INDEX_H
