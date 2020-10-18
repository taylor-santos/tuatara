#include "json.h"

using namespace std;

int  JSON::JSON::JSON::indent   = 0;
bool JSON::JSON::JSON::minimize = true;

JSON::JSON::JSON(std::ostream &out)
    : out{out} {}

void
JSON::JSON::Key(const std::string &key) {
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
JSON::JSON::Indent() {
    out << string(indent, '\t');
}

JSON::Object::Object(ostream &out)
    : JSON(out) {
    out << "{";
    indent++;
}

JSON::Object::~Object() {
    indent--;
    if (!minimize && !first) {
        out << endl;
        Indent();
    }
    out << "}";
}

void
JSON::Object::KeyValue(const std::string &key, const std::string &value) {
    Key(key);
    out << '"' << value << '"';
}

void
JSON::Object::KeyValue(const string &key, const char *value) {
    Key(key);
    out << '"' << value << '"';
}

void
JSON::Object::KeyValue(const string &key, bool value) {
    Key(key);
    out << std::boolalpha << value;
}

JSON::Array::Array(ostream &out)
    : JSON(out) {
    out << "[";
    indent++;
}

JSON::Array::~Array() {
    indent--;
    if (!minimize && !first) {
        out << endl;
        Indent();
    }
    out << "]";
}

void
JSON::Array::Next() {
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
JSON::Array::String(const string &str) {
    Next();
    out << '"' << str << '"';
}
