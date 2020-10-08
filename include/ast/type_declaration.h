#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H

#include "declaration.h"

#include <memory>

namespace AST {

class TypeDeclaration final : public Declaration {
protected:
    void json(std::ostream &os) const override;

public:
    TypeDeclaration(const yy::location &loc, std::string variable);
};

} // namespace AST

#endif // TYPE_DECLARATION_H
