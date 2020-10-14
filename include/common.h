#ifndef COMMON_H
#define COMMON_H

#include "ast/int.h"
#include "ast/float.h"
#include "ast/string.h"
#include "ast/variable.h"
#include "ast/typed_variable.h"
#include "ast/assignment.h"
#include "ast/type_declaration.h"
#include "ast/value_declaration.h"
#include "ast/type_value_declaration.h"

#include "type/object.h"

using ExpressionPtr = std::unique_ptr<AST::Expression>;
using LValuePtr     = std::unique_ptr<AST::Statement>;
using StatementPtr  = std::unique_ptr<AST::Statement>;
using TypePtr       = std::shared_ptr<TypeChecker::Type>;
using StatementVec  = std::vector<StatementPtr>;

#endif // COMMON_H
