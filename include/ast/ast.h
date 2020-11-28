#ifndef AST_AST_H
#define AST_AST_H

#include <iostream> // std::ostream
#include <vector>
#include <memory>

#include "location.hh"

// Namespace and class must be forward declared for operator<< to work properly
namespace AST {
class Node;
}
std::ostream &operator<<(std::ostream &os, const AST::Node &ast);

namespace AST {

class Node {
public: // Aliases
    using Ptr = std::unique_ptr<Node>;
    using Vec = std::vector<Ptr>;

public: // Methods
    virtual ~Node() = default;
    friend std::ostream & ::operator<<(std::ostream &os, const Node &ast);

protected: // Methods
    explicit Node(const yy::location &loc);
    virtual void json(std::ostream &os) const = 0;

private: // Fields
    yy::location loc;
};

} // namespace AST

#endif // AST_AST_H
