#ifndef TYPE_CONTEXT_H
#define TYPE_CONTEXT_H

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace TypeChecker {

class Type;
class Func;
class Class;

class Context {
public: // Methods
    Context();
    Context(const Context &other) = default;
    ~Context();
    auto
    operator=(const Context &) = delete;
    [[nodiscard]] std::optional<std::shared_ptr<TypeChecker::Type>>
    getSymbol(const std::string &name);
    void
    setSymbol(const std::string &name, const std::shared_ptr<TypeChecker::Type> &type);
    void
    updateSymbol(const std::string &name, const std::shared_ptr<TypeChecker::Type> &type);
    void
    addClass(const std::string &name, Class &cl);
    [[nodiscard]] Class *
    getClass(const std::string &name) const;
    const std::map<std::string, std::shared_ptr<TypeChecker::Type>> &
    getSymbols() const;
    const std::unordered_map<std::string, std::reference_wrapper<Class>> &
    getClasses() const;
    void
    printSymbols(std::ostream &out) const;
    void
    printClasses(std::ostream &out) const;

private: // Fields
    std::map<std::string, std::shared_ptr<TypeChecker::Type>>      symbols_;
    std::unordered_map<std::string, std::reference_wrapper<Class>> classes_;
};

} // namespace TypeChecker

#endif // TYPE_CONTEXT_H
