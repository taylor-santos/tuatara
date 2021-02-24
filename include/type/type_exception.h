#ifndef TYPE_EXCEPTION_H
#define TYPE_EXCEPTION_H

#include <exception>
#include <string>
#include <vector>

namespace yy {
class location;
}

namespace TypeChecker {

class TypeException final : public std::exception {
public: // Methods
    TypeException(const std::string &msg, const yy::location &loc);
    explicit TypeException(std::vector<std::pair<std::string, yy::location>> msgs);
    [[nodiscard]] const std::vector<std::pair<std::string, yy::location>> &getMsgs() const;

private: // Fields
    std::vector<std::pair<std::string, yy::location>> msgs_;
};

} // namespace TypeChecker

#endif // TYPE_EXCEPTION_H
