#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <functional>

namespace JSON {

class JSON {
protected:
    std::ostream &out;
    bool          first = true;
    static int    indent;

protected:
    explicit JSON(std::ostream &out);
    virtual ~JSON() = default;
    void Indent();

public:
    static bool minimize;

public:
    void Key(const std::string &key);
};

class Object final : public JSON {
public:
    explicit Object(std::ostream &out = std::cout);
    ~Object() override;
    template<class T>
    void KeyValue(const std::string &key, const T &value);
    void KeyValue(const std::string &key, const std::string &value);
    void KeyValue(const std::string &key, const char value[]);
    void KeyValue(const std::string &key, bool value);
};

class Array final : public JSON {
public:
    explicit Array(std::ostream &out = std::cout);
    ~Array() override;
    void Next();
    void String(const std::string &str);
    template<class T>
    Array &operator<<(const T &container);
};

template<class T>
void
Object::KeyValue(const std::string &key, const T &value) {
    Key(key);
    out << value;
}

template<class T>
Array &
Array::operator<<(const T &container) {
    for (const auto &t : container) {
        Next();
        out << *t;
    }
    return *this;
}

} // namespace JSON

#endif // JSON_H
