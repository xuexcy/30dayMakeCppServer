#pragma once

#include <string>

class Buffer {
public:
    Buffer() = default;
    ~Buffer() = default;
    void Append(const char* str, int size);
    ssize_t Size()const;
    const char* ToStr();

    void Clear();
    void Getline();
    void SetBuf(const char* buf);
private:
    std::string buf_;
};
