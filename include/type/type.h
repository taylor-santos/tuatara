#ifndef TYPE_TYPE_H
#define TYPE_TYPE_H

#include "ast.h"

namespace yy {
class location;
}

// Forward declaration
namespace AST {
class Expression;
class Call;
} // namespace AST

namespace TypeChecker {

class Context;

class Type
    : public AST::Node
    , public std::enable_shared_from_this<Type> {
public: // Methods
    explicit Type(yy::location loc);
    ~Type() override;
    /// Verify that this type definition is valid in the given context,
    /// and update any needed fields in response.
    /// \param ctx The type checker context
    void         verify(Context &ctx);
    void         pretty(std::ostream &out) const;
    virtual void pretty(std::ostream &out, bool mod) const = 0;
    virtual std::shared_ptr<Type>
                       callAsFunc(Context &ctx, AST::Expression &arg, const AST::Call &call);
    virtual bool       isSubtype(const Type &other, const Context &ctx) const = 0;
    bool               isSupertype(const Type &other, const Context &ctx) const;
    bool               isEqual(const Type &other, Context &ctx) const;
    Type &             operator=(const Type &) = delete;
    [[nodiscard]] bool isInitialized() const;
    void               setInitialized(bool initialized);

protected: // Enum Classes
    enum class VerifyState { NONE, FAILED, VERIFIED };

private: // Fields
    bool        initialized_ = false;
    VerifyState verifyState_ = VerifyState::NONE;

private: // Methods
    virtual void                  verifyImpl(Context &ctx) = 0;
    virtual std::shared_ptr<Type> simplify(Context &ctx);
    void                          setVerifyState(VerifyState verifyState);
    friend class Array;
    virtual bool isSuperImpl(const class Array &other, const Context &ctx) const;
    friend class Class;
    virtual bool isSuperImpl(const class Class &other, const Context &ctx) const;
    friend class Func;
    virtual bool isSuperImpl(const class Func &other, const Context &ctx) const;
    friend class Maybe;
    virtual bool isSuperImpl(const class Maybe &other, const Context &ctx) const;
    friend class Object;
    virtual bool isSuperImpl(const class Object &other, const Context &ctx) const;
    friend class Product;
    virtual bool isSuperImpl(const class Product &other, const Context &ctx) const;
    friend class Sum;
    virtual bool isSuperImpl(const class Sum &other, const Context &ctx) const;
    friend class Unit;
    virtual bool isSuperImpl(const class Unit &other, const Context &ctx) const;
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
