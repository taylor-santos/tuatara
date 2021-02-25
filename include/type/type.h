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
public: // Methods
    explicit Type(yy::location loc);
    ~Type() override;
    /// Verify that this type definition is valid in the given context,
    /// and update any needed fields in response.
    /// \param ctx The type checker context
    void          verify(class Context &ctx);
    void          pretty(std::ostream &out) const;
    virtual void  pretty(std::ostream &out, bool mod) const = 0;
    virtual Type &callAsFunc(class Context &ctx, AST::Expression &arg);
    virtual bool  isSubtype(const Type &other) const   = 0;
    virtual bool  isSupertype(const Type &other) const = 0;
    Type &        operator=(const Type &) = delete;

protected: // Methods
    virtual void verifyImpl(class Context &ctx) = 0;
    friend class Array;
    virtual bool isSupertype(const class Array &other) const;
    friend class Class;
    virtual bool isSupertype(const class Class &other) const;
    friend class Func;
    virtual bool isSupertype(const class Func &other) const;
    friend class Maybe;
    virtual bool isSupertype(const class Maybe &other) const;
    friend class Object;
    virtual bool isSupertype(const class Object &other) const;
    friend class Product;
    virtual bool isSupertype(const class Product &other) const;
    friend class Sum;
    virtual bool isSupertype(const class Sum &other) const;
    friend class Unit;
    virtual bool isSupertype(const class Unit &other) const;

private: // Fields
    enum class VerifyState { NONE, FAILED, VERIFIED } verifyState_ = VerifyState::NONE;
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
