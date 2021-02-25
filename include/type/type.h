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

class Context;

class Type : public AST::Node {
public: // Methods
    explicit Type(yy::location loc);
    ~Type() override;
    /// Verify that this type definition is valid in the given context,
    /// and update any needed fields in response.
    /// \param ctx The type checker context
    void          verify(Context &ctx);
    void          pretty(std::ostream &out) const;
    virtual void  pretty(std::ostream &out, bool mod) const = 0;
    virtual Type &callAsFunc(Context &ctx, AST::Expression &arg);
    virtual bool  isSubtype(const Type &other, Context &ctx) const   = 0;
    virtual bool  isSupertype(const Type &other, Context &ctx) const = 0;
    Type &        operator=(const Type &) = delete;

protected: // Enum Classes
    enum class VerifyState { NONE, FAILED, VERIFIED };

protected: // Methods
    virtual void verifyImpl(Context &ctx) = 0;
    void         setVerifyState(VerifyState verifyState);
    friend class Array;
    virtual bool isSupertype(const class Array &other, Context &ctx) const;
    friend class Class;
    virtual bool isSupertype(const class Class &other, Context &ctx) const;
    friend class Func;
    virtual bool isSupertype(const class Func &other, Context &ctx) const;
    friend class Maybe;
    virtual bool isSupertype(const class Maybe &other, Context &ctx) const;
    friend class Object;
    virtual bool isSupertype(const class Object &other, Context &ctx) const;
    friend class Product;
    virtual bool isSupertype(const class Product &other, Context &ctx) const;
    friend class Sum;
    virtual bool isSupertype(const class Sum &other, Context &ctx) const;
    friend class Unit;
    virtual bool isSupertype(const class Unit &other, Context &ctx) const;

private: // Fields
    VerifyState verifyState_ = VerifyState::NONE;
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
