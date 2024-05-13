#include "buffer.h"

#include <iostream>
#include <string>

void Buffer::append(const char* str, int size) {
    for (int i = 0; i < size; ++i) {
        if (str[i] == '\0') break;
        buf.push_back(str[i]);
    }
}

ssize_t Buffer::size() const {
    return buf.size();
}

const char* Buffer::c_str() {
    return buf.c_str();
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getline() {
    clear();
    std::getline(std::cin, buf);
}
