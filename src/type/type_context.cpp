#include "type/type_context.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <utility>

#include "type/class.h"
#include "type/func.h"
#include "type/object.h"
#include "type/product.h"
#include "type/type_exception.h"
#include "type/unit.h"

using std::endl;
using std::left;
using std::make_shared;
using std::make_unique;
using std::max;
using std::max_element;
using std::nullopt;
using std::optional;
using std::pair;
using std::reference_wrapper;
using std::right;
using std::setw;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace TypeChecker {

static unordered_map<string, shared_ptr<Class>> BUILTINS = Class::generateBuiltins();

Context::Context() {
    for (const auto &[name, type] : BUILTINS) {
        addClass(name, *type);
    }
    for (auto &[_, type] : symbols_) {
        type = TypeChecker::Type::verify(type, *this);
    }
}

Context::~Context() = default;

optional<shared_ptr<TypeChecker::Type>>
Context::getSymbol(const string &name) {
    auto it = symbols_.find(name);
    if (it == symbols_.end()) {
        return nullopt;
    }
    return it->second;
}

void
Context::setSymbol(const string &name, const shared_ptr<TypeChecker::Type> &type) {
    symbols_[name] = type;
}

void
Context::updateSymbol(const std::string &name, const std::shared_ptr<TypeChecker::Type> &type) {
    const auto [prev, added] = symbols_.try_emplace(name, type);
    if (!added) {
        const auto &[n, prevType] = *prev;
        if (type->isSubtype(*prevType, *this)) {
            prevType->setInitialized(type->isInitialized());
            return;
        }
        vector<pair<string, yy::location>> msgs;
        {
            stringstream ss;
            ss << "error: redefining variable \"" << name << "\" to have type \"";
            type->pretty(ss);
            ss << "\"";
            msgs.emplace_back(ss.str(), type->getLoc());
        }
        {
            stringstream ss;
            ss << "note: \"" << name << "\" defined to have type \"";
            prevType->pretty(ss);
            ss << "\"";
            msgs.emplace_back(ss.str(), prevType->getLoc());
        }
        throw TypeChecker::TypeException(msgs);
    }
}

void
Context::addClass(const string &name, Class &cl) {
    classes_.emplace(name, cl);
}

Class *
Context::getClass(const string &name) const {
    auto it = classes_.find(name);
    if (it == classes_.end()) {
        return nullptr;
    }
    return &it->second.get();
}

const std::map<string, std::shared_ptr<TypeChecker::Type>> &
Context::getSymbols() const {
    return symbols_;
}

const unordered_map<string, reference_wrapper<Class>> &
Context::getClasses() const {
    return classes_;
}

void
Context::printSymbols(std::ostream &out) const {
    size_t maxLen =
        max_element(getSymbols().begin(), getSymbols().end(), [](const auto &s1, const auto &s2) {
            return s1.first.length() < s2.first.length();
        })->first.length();
    string label = "Name";
    maxLen       = max(maxLen, label.length());
    out << setw(maxLen) << left << "Name"
        << " | "
        << "Type" << endl;
    out << "-----|-----" << endl;
    for (const auto &[name, type] : getSymbols()) {
        out << setw(maxLen) << left << name << " | ";
        type->pretty(out);
        out << endl;
    }
}

void
Context::printClasses(std::ostream &out) const {
    size_t maxLen =
        max_element(getClasses().begin(), getClasses().end(), [](const auto &c1, const auto &c2) {
            return c1.first.length() < c2.first.length();
        })->first.length();
    maxLen = 2 * (maxLen / 2);
    out << string(maxLen + 1, ' ');
    string sep = "╔";
    for (size_t i = 0; i < getClasses().size(); i++) {
        out << sep;
        sep = "╤";
        for (size_t j = 0; j < maxLen; j++) {
            out << "═";
        }
    }
    out << "╗" << endl;
    out << string(maxLen + 1, ' ');
    sep = "║";
    for (const auto &[name, cl] : getClasses()) {
        auto n = (maxLen - name.length()) / 2;
        out << sep << string(n, ' ') << name << string(maxLen - name.length() - n, ' ');
        sep = "│";
    }
    out << "║" << endl;
    sep = "╔";
    for (size_t i = 0; i <= getClasses().size(); i++) {
        out << sep;
        sep = i == 0 ? "╬" : "╪";
        for (size_t j = 0; j < maxLen; j++) {
            out << "═";
        }
    }
    out << "╣" << endl;

    for (const auto &[n1, c1] : getClasses()) {
        out << "║" << setw(maxLen) << right << n1;
        sep = "║";
        for (const auto &[n2, c2] : getClasses()) {
            auto n = (maxLen - 1) / 2;
            out << sep << string(n, ' ');
            sep        = "│";
            bool sub   = c1.get().isSubtype(c2.get(), *this);
            bool super = c1.get().isSupertype(c2.get(), *this);
            if (sub) {
                if (super) {
                    out << "=";
                } else {
                    out << "<";
                }
            } else if (super) {
                out << ">";
            } else {
                out << " ";
            }
            out << string(maxLen - n - 1, ' ');
        }
        out << "║" << endl;
    }
    sep = "╚";
    for (size_t i = 0; i <= getClasses().size(); i++) {
        out << sep;
        sep = i == 0 ? "╩" : "╧";
        for (size_t j = 0; j < maxLen; j++) {
            out << "═";
        }
    }
    out << "╝" << endl;
}

} // namespace TypeChecker
