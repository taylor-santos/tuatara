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
protected:
    yy::location loc;

protected:
    Node(const yy::location &loc);
    virtual void json(std::ostream &os) const = 0;

public:
    virtual ~Node() = default;
    friend std::ostream & ::operator<<(std::ostream &os, const Node &ast);
};

} // namespace AST

#endif // AST_H
