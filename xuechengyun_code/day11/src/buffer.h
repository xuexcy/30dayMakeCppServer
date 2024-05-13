#pragma once

#include <string>

class Buffer {
public:
    Buffer() = default;
    ~Buffer() = default;
    void append(const char* str, int size);
    ssize_t size()const;
    const char* c_str();

    void clear();
    void getline();
private:
    std::string buf;
};
