#ifndef TYPE_TYPE_H
#define TYPE_TYPE_H

#include "ast.h"

namespace yy {
class location;
}

// Forward declaration
namespace AST {
class Expression;
}

namespace TypeChecker {

class Type : public AST::Node {
public: // Aliases
    using Ptr = std::unique_ptr<Type>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Type(yy::location loc);
    Type(const Type &) = delete;
    /// Verify that this type definition is valid in the given context,
    /// and update any needed fields in response.
    /// \param ctx The type checker context
    void          verify(class Context &ctx);
    void          pretty(std::ostream &out) const;
    virtual void  pretty(std::ostream &out, bool mod) const = 0;
    virtual Type &callAsFunc(class Context &ctx, AST::Expression &arg);
    virtual bool  operator<=(const Type &other) const = 0;
    Type &        operator=(const Type &) = delete;

protected: // Methods
    virtual void verifyImpl(class Context &ctx) = 0;
    virtual bool operator>=(const Type &other) const;
    friend class Array;
    virtual bool operator>=(const class Array &other) const;
    friend class Class;
    virtual bool operator>=(const class Class &other) const;
    friend class Func;
    virtual bool operator>=(const class Func &other) const;
    friend class Maybe;
    virtual bool operator>=(const class Maybe &other) const;
    friend class Object;
    virtual bool operator>=(const class Object &other) const;
    friend class Product;
    virtual bool operator>=(const class Product &other) const;
    friend class Sum;
    virtual bool operator>=(const class Sum &other) const;
    friend class Unit;
    virtual bool operator>=(const class Unit &other) const;

private: // Fields
    enum class VerifyState { NONE, FAILED, VERIFIED } verifyState_ = VerifyState::NONE;
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
