#ifndef AST_STUB_H
#define AST_STUB_H

#include "ast.h"

namespace AST {

class Stub final : public Node {
public: // Aliases
    using Ptr = std::unique_ptr<Stub>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Stub(const yy::location &loc, std::string data);
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    std::string data_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_STUB_H
