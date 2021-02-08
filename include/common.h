#ifndef COMMON_H
#define COMMON_H

#include "ast/int.h"
#include "ast/float.h"
#include "ast/string.h"
#include "ast/bool.h"
#include "ast/variable.h"
#include "ast/type_declaration.h"
#include "ast/value_declaration.h"
#include "ast/type_value_declaration.h"
#include "ast/func_declaration.h"
#include "ast/func_impl.h"
#include "ast/class_declaration.h"
#include "ast/return.h"
#include "ast/call.h"
#include "ast/index.h"
#include "ast/block.h"
#include "ast/while.h"
#include "ast/if.h"
#include "ast/tuple.h"
#include "ast/operator.h"

#include "type/sum.h"
#include "type/object.h"
#include "type/product.h"
#include "type/array.h"
#include "type/func.h"

using OptType      = std::optional<TypeChecker::Type::Ptr>;
using NamedType    = TypeChecker::Type::Named;
using NamedTypeVec = std::vector<NamedType>;

#endif // COMMON_H
