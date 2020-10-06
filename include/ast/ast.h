#ifndef AST_H
#define AST_H

#include <iostream> // std::ostream

#include "location.hh"

// Namespace and class must be forward declared for operator<< to work properly
namespace TuataraAST {
class AST;
}
std::ostream &operator<<(std::ostream &os, const TuataraAST::AST &ast);

namespace TuataraAST {

class AST {
protected:
    yy::location loc;

protected:
    AST(const yy::location &loc);
    virtual void json(std::ostream &os) const = 0;

public:
    virtual ~AST() = default;
    friend std::ostream & ::operator<<(std::ostream &os, const AST &ast);
};

} // namespace TuataraAST

#endif // AST_H
