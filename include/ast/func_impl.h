#ifndef AST_FUNC_IMPL_H
#define AST_FUNC_IMPL_H

#include "ast/block.h"
#include "ast/func_declaration.h"

namespace AST {

class FuncImpl final : public FuncDeclaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncImpl>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncImpl(
        const yy::location &                  loc,
        std::string                           variable,
        Pattern::Pattern::Vec                 args,
        Block::Ptr                            body,
        std::optional<TypeChecker::Type::Ptr> retType = {});
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Block::Ptr body_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FUNC_IMPL_H
