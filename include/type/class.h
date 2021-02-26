#ifndef TYPE_CLASS_H
#define TYPE_CLASS_H

#include <unordered_map>

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Class final : public Type {
public: // Methods
    Class(yy::location loc, std::string name);
    ~Class() override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    Type *                           getField(const std::string &name) const;
    [[nodiscard]] bool               addField(const std::string &name, std::unique_ptr<Type> type);
    const std::string &              getClassName() const;
    void                             addSubType(Class *cl);
    void                             addSuperType(Class *cl);
    bool                             isSubtype(const Type &other, Context &ctx) const override;

    static std::unordered_map<std::string, std::unique_ptr<Class>> generateBuiltins();


private: // Fields
    std::string                                            name_;
    std::unordered_map<std::string, std::unique_ptr<Type>> fields_;
    enum class ClassRelation { SUBTYPE, SUPERTYPE };
    std::unordered_map<const Class *, ClassRelation> classRltns_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
    void addSubTypeImpl(Class *cl);
    void addSuperTypeImpl(Class *cl);
    bool isSuperImpl(const class Class &other, Context &ctx) const override;
};

} // namespace TypeChecker

#endif // TYPE_CLASS_H
