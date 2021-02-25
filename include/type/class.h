#ifndef TYPE_CLASS_H
#define TYPE_CLASS_H

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
    bool                             operator<=(const Type &other) const override;
    Type *                           getField(const std::string &name) const;
    [[nodiscard]] bool               addField(const std::string &name, std::unique_ptr<Type> type);
    const std::string &              getClassName() const;

protected: // Methods
    bool operator>=(const Class &other) const override;

private: // Fields
    std::string                                            name_;
    std::unordered_map<std::string, std::unique_ptr<Type>> fields_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_CLASS_H
