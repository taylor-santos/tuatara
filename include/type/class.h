#ifndef TYPE_CLASS_H
#define TYPE_CLASS_H

#include <optional>
#include <unordered_map>

#include "type/type.h"

namespace TypeChecker {

class Class final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Class>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Class(yy::location loc, std::string name);
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;
    Type *                           getField(const std::string &name) const;
    [[nodiscard]] bool               addField(const std::string &name, Type::Ptr type);
    const std::string &              getClassName() const;

protected: // Methods
    bool operator>=(const Class &other) const override;

private: // Fields
    std::string                                name_;
    std::unordered_map<std::string, Type::Ptr> fields_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_CLASS_H
