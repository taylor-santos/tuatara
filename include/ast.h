#ifndef AST_H
#define AST_H

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "location.hh"

namespace AST {

class Node {
public: // Aliases
    using Ptr  = std::unique_ptr<Node>;
    using Vec  = std::vector<Ptr>;
    using Func = std::function<void(const Node &)>;

public: // Methods
    virtual ~Node() = default;
    virtual void                             walk(const Func &fn) const;
    friend std::ostream &                    operator<<(std::ostream &os, const Node &ast);
    [[nodiscard]] const yy::location &       getLoc() const;
    [[nodiscard]] virtual const std::string &getTypeName() const = 0;

protected: // Methods
    explicit Node(const yy::location &loc);
    virtual void json(std::ostream &os) const = 0;

private: // Fields
    yy::location loc_;
};

} // namespace AST

#endif // AST_H
