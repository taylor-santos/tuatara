#ifndef FUNC_DECLARATION_H
#define FUNC_DECLARATION_H

#include "declaration.h"
#include "type/type.h"

#include <memory>
#include <vector>
#include <optional>

namespace AST {

class FuncDeclaration final : public Declaration {
public: // Methods
    FuncDeclaration(
        const yy::location &                                                    loc,
        std::string                                                             variable,
        std::vector<std::pair<std::string, std::shared_ptr<TypeChecker::Type>>> args,
        std::optional<std::shared_ptr<TypeChecker::Type>>                       ret_type,
        std::unique_ptr<Statement>                                              body);

private: // Fields
    std::vector<std::pair<std::string, std::shared_ptr<TypeChecker::Type>>> args;
    std::optional<std::shared_ptr<TypeChecker::Type>>                       ret_type;
    std::unique_ptr<Statement>                                              body;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // FUNC_DECLARATION_H
