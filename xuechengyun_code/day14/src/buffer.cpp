#include "buffer.h"

#include <iostream>
#include <string>

void Buffer::Append(const char* str, int size) {
    for (int i = 0; i < size; ++i) {
        if (str[i] == '\0') break;
        buf_.push_back(str[i]);
    }
}

ssize_t Buffer::Size() const {
    return buf_.size();
}

const char* Buffer::ToStr() {
    return buf_.c_str();
}

void Buffer::Clear() {
    buf_.clear();
}

void Buffer::Getline() {
    Clear();
    std::getline(std::cin, buf_);
}

void Buffer::SetBuf(const char* buf) {
    buf_.clear();
    buf_.append(buf);
}
