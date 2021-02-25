#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "ast/expression.h"

namespace TypeChecker {
class Object;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Literal : public Expression {
public: // Methods
    explicit Literal(const yy::location &loc, std::string className);
    ~Literal() override;

protected: // Methods
    [[nodiscard]] TypeChecker::Object &getMyType();

private: // Fields
    std::unique_ptr<TypeChecker::Object> myType_;
};

} // namespace AST

#endif // AST_LITERAL_H
