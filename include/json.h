#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

namespace JSON {

class JSON {
protected:
    std::ostream &out_;
    bool          first_ = true;
    static int    indent_;
    static bool   minimize_;

protected:
    explicit JSON(std::ostream &out);
    virtual ~JSON() = default;
    void indent();

public:
    static void setMinimize(bool minimize);

public:
    void printKey(const std::string &key);
};

class Object final : public JSON {
public:
    explicit Object(std::ostream &out = std::cout);
    ~Object() override;
    template<class T>
    void printKeyValue(const std::string &key, const T &value);
    template<class T>
    void printKeyValue(const std::string &key, const std::unique_ptr<T> &value);
    template<class T>
    void printKeyValue(const std::string &key, const std::optional<T> &value);
    template<class T>
    void printKeyValue(const std::string &key, const std::vector<T> &value);
    void printKeyValue(const std::string &key, const std::string &value);
    void printKeyValue(const std::string &key, const char *value);
    void printKeyValue(const std::string &key, bool value);
};

class Array final : public JSON {
public:
    explicit Array(std::ostream &out = std::cout);
    ~Array() override;
    void next();
    template<class T>
    void printValue(const T &value);
    template<class T>
    void printValue(const std::unique_ptr<T> &value);
    void printValue(const std::string &str);
    void printValue(const char *value);
};

template<class T>
void
Object::printKeyValue(const std::string &key, const T &value) {
    printKey(key);
    out_ << value;
}

template<class T>
void
Object::printKeyValue(const std::string &key, const std::unique_ptr<T> &value) {
    printKeyValue(key, *value);
}

template<class T>
void
Object::printKeyValue(const std::string &key, const std::optional<T> &value) {
    if (value) {
        printKeyValue(key, *value);
    }
}

template<class T>
void
Object::printKeyValue(const std::string &key, const std::vector<T> &value) {
    printKey(key);
    Array arr(out_);
    for (const T &x : value) {
        arr.printValue(x);
    }
}

template<class T>
void
Array::printValue(const T &value) {
    next();
    out_ << value;
}

template<class T>
void
Array::printValue(const std::unique_ptr<T> &value) {
    printValue(*value);
}

} // namespace JSON

#endif // JSON_H
