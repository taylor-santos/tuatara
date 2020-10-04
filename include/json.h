#ifndef JSON_H
#define JSON_H

#include <iostream>

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
    void String(const std::string &key, const std::string &str);
};

class Array final : public JSON {
public:
    explicit Array(std::ostream &out = std::cout);
    ~Array() override;
    void Next();
    void String(const std::string &str);
};

} // namespace JSON

#endif // JSON_H