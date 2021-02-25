#ifndef AST_INDEX_H
#define AST_INDEX_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Index final : public LValue {
public: // Methods
    Index(
        const yy::location &        loc,
        std::unique_ptr<Expression> expr,
        std::unique_ptr<Expression> index);
    ~Index() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Expression> expr_;
    std::unique_ptr<Expression> index_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_INDEX_H
