#ifndef AST_H
#define AST_H

#include <iostream> // std::ostream

#include "location.hh"

// Namespace and class must be forward declared for operator<< to work properly
namespace AST {
class Node;
}
std::ostream &operator<<(std::ostream &os, const AST::Node &ast);

namespace AST {

class Node {
protected: // Fields
    yy::location loc;

protected: // Methods
    explicit Node(const yy::location &loc);
    virtual void json(std::ostream &os) const = 0;

public: // Methods
    virtual ~Node() = default;
    friend std::ostream & ::operator<<(std::ostream &os, const Node &ast);
};

} // namespace AST

#endif // AST_H
