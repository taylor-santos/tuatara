#ifndef COMMON_H
#define COMMON_H

#include "ast/block.h"
#include "ast/bool.h"
#include "ast/call.h"
#include "ast/float.h"
#include "ast/func_declaration.h"
#include "ast/func_impl.h"
#include "ast/if.h"
#include "ast/if_else.h"
#include "ast/index.h"
#include "ast/infix_operator.h"
#include "ast/int.h"
#include "ast/lambda.h"
#include "ast/string.h"
#include "ast/stub.h"
#include "ast/tuple.h"
#include "ast/type_declaration.h"
#include "ast/type_value_declaration.h"
#include "ast/unit.h"
#include "ast/value_declaration.h"
#include "ast/variable.h"
#include "ast/while.h"

#include "pattern/literal.h"
#include "pattern/named_constraint.h"
#include "pattern/named_wildcard.h"
#include "pattern/tuple.h"
#include "pattern/type_constraint.h"
#include "pattern/value_constraint.h"

#include "type/array.h"
#include "type/func.h"
#include "type/maybe.h"
#include "type/object.h"
#include "type/product.h"
#include "type/sum.h"
#include "type/unit.h"

using OptType      = std::optional<TypeChecker::Type::Ptr>;
using NamedType    = TypeChecker::Type::Named;
using NamedTypeVec = std::vector<NamedType>;

#endif // COMMON_H
