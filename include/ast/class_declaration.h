#ifndef AST_CLASS_DECLARATION_H
#define AST_CLASS_DECLARATION_H

#include "declaration.h"
#include "func_declaration.h"
#include "type/type.h"

#include <optional>

namespace AST {

class ClassDeclaration : public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<ClassDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Classes
    struct Field {
        std::string            name;
        TypeChecker::Type::Ptr type;
    };
    struct Operator {
        std::string                           operation;
        TypeChecker::Type::Named              arg;
        std::optional<TypeChecker::Type::Ptr> retType;
    };
    struct Constructor {
        std::vector<TypeChecker::Type::Named> args;
    };
    struct Members {
        std::vector<Field>                fields;
        std::vector<FuncDeclaration::Ptr> methods;
        std::vector<Operator>             operators;
        std::vector<Constructor>          ctors;
    };

public: // Methods
    ClassDeclaration(
        const yy::location &     loc,
        std::string              name,
        std::vector<std::string> supers,
        Members                  members);

protected: // Methods
    void json(std::ostream &os) const override;

private: // Fields
    std::vector<std::string> supers;
    Members                  members;
};

} // namespace AST

#endif // AST_CLASS_DECLARATION_H
