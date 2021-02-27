#include "json.h"

#include <algorithm>
#include <sstream>

using std::boolalpha;
using std::endl;
using std::hex;
using std::ostream;
using std::string;
using std::stringstream;
using std::uppercase;

namespace JSON {

int  JSON::indent_   = 0;
bool JSON::minimize_ = true;

JSON::JSON(ostream &out)
    : out_{out} {}

void
JSON::printKey(const string &key) {
    if (!first_) {
        out_ << ',';
    }
    if (!minimize_) {
        out_ << endl;
        indent();
    }
    first_ = false;
    out_ << '"' << key << '"' << ':';
    if (!minimize_) {
        out_ << " ";
    }
}

void
JSON::indent() {
    out_ << string(indent_, '\t');
}

void
JSON::setMinimize(bool minimize) {
    minimize_ = minimize;
}

Object::Object(ostream &out)
    : JSON(out) {
    out << "{";
    indent_++;
}

Object::~Object() {
    indent_--;
    if (!minimize_ && !first_) {
        out_ << endl;
        indent();
    }
    out_ << "}";
}

static string
escapeChar(char c) {
    if (isprint(static_cast<unsigned char>(c))) {
        return {c};
    }
    switch (c) {
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        case '\\': return "\\\\";
        case '\'': return "\\\'";
        case '\"': return "\\\"";
        case '\?': return "\\?";
        default:
            stringstream ss;
            ss << "\\x" << hex << uppercase;
            if (c < 0) {
                ss << c + 256;
            } else {
                ss << c;
            }
            return ss.str();
    }
}

static string
escape(const string &str) {
    string s;
    for_each(str.begin(), str.end(), [&](const auto &c) { s += escapeChar(c); });
    return s;
}

void
Object::printKeyValue(const string &key, const string &value) {
    printKey(key);
    out_ << '"' << escape(value) << '"';
}

void
Object::printKeyValue(const string &key, const char *value) {
    printKey(key);
    out_ << '"' << escape(value) << '"';
}

void
Object::printKeyValue(const string &key, bool value) {
    printKey(key);
    out_ << boolalpha << value;
}

Array::Array(ostream &out)
    : JSON(out) {
    out << "[";
    indent_++;
}

Array::~Array() {
    indent_--;
    if (!minimize_ && !first_) {
        out_ << endl;
        indent();
    }
    out_ << "]";
}

void
Array::next() {
    if (!first_) {
        out_ << ',';
    }
    if (!minimize_) {
        out_ << endl;
        indent();
    }
    first_ = false;
}

void
Array::printValue(const string &str) {
    next();
    out_ << '"' << str << '"';
}

void
Array::printValue(const char *str) {
    next();
    out_ << '"' << str << '"';
}

} // namespace JSON
