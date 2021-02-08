#include "json.h"

using namespace std;

namespace JSON {

int  JSON::indent   = 0;
bool JSON::minimize = true;

JSON::JSON(ostream &out)
    : out{out} {}

void
JSON::Key(const string &key) {
    if (!first) {
        out << ',';
    }
    if (!minimize) {
        out << endl;
        Indent();
    }
    first = false;
    out << '"' << key << '"' << ':';
    if (!minimize) {
        out << " ";
    }
}

void
JSON::Indent() {
    out << string(indent, '\t');
}

Object::Object(ostream &out)
    : JSON(out) {
    out << "{";
    indent++;
}

Object::~Object() {
    indent--;
    if (!minimize && !first) {
        out << endl;
        Indent();
    }
    out << "}";
}

void
Object::KeyValue(const string &key, const string &value) {
    Key(key);
    out << '"' << value << '"';
}

void
Object::KeyValue(const string &key, const char *value) {
    Key(key);
    out << '"' << value << '"';
}

void
Object::KeyValue(const string &key, bool value) {
    Key(key);
    out << boolalpha << value;
}

Array::Array(ostream &out)
    : JSON(out) {
    out << "[";
    indent++;
}

Array::~Array() {
    indent--;
    if (!minimize && !first) {
        out << endl;
        Indent();
    }
    out << "]";
}

void
Array::Next() {
    if (!first) {
        out << ',';
    }
    if (!minimize) {
        out << endl;
        Indent();
    }
    first = false;
}

void
Array::String(const string &str) {
    Next();
    out << '"' << str << '"';
}

} // namespace JSON
