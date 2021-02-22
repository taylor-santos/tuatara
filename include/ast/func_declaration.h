#ifndef AST_FUNC_DECLARATION_H
#define AST_FUNC_DECLARATION_H

#include <functional>
#include <optional>

#include "ast/declaration.h"

#include "pattern/pattern.h"

#include "type/type.h"

namespace AST {

class FuncDeclaration : public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncDeclaration(
        const yy::location &                  loc,
        std::string                           variable,
        Pattern::Pattern::Vec                 args,
        std::optional<TypeChecker::Type::Ptr> retType = {});
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

protected: // Methods
    [[nodiscard]] const Pattern::Pattern::Vec &                getArgs() const;
    [[nodiscard]] const std::optional<TypeChecker::Type::Ptr> &getRetType() const;

private: // Fields
    Pattern::Pattern::Vec                 args_;
    std::optional<TypeChecker::Type::Ptr> retType_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_FUNC_DECLARATION_H
