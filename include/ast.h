#ifndef AST_H
#define AST_H

#define STRINGIFY_(X) #X
#define STRINGIFY(X)  STRINGIFY_(X)
#define LOC_STR       __FILE__ ":" STRINGIFY(__LINE__)

#include <functional>
#include <memory>

#include "location.hh"

namespace AST {

class Node {
public: // Methods
    Node(const Node &) = delete;
    virtual ~Node();
    virtual void                      walk(const std::function<void(const Node &)> &fn) const;
    friend std::ostream &             operator<<(std::ostream &os, const Node &ast);
    [[nodiscard]] const yy::location &getLoc() const;
    void                              setLoc(const yy::location &loc);
    [[nodiscard]] virtual const std::string &getNodeName() const = 0;

protected: // Methods
    explicit Node(const yy::location &loc);
    virtual void json(std::ostream &os) const = 0;

private: // Fields
    yy::location loc_;
};

} // namespace AST

#endif // AST_H
