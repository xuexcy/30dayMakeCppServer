#pragma once

#include <format>
#include <iostream>

void exit_if(bool condition, const char* errmsg);

#define CoutFormat(...) std::cout << std::format(__VA_ARGS__);
// template <typename Fmt, typename... Args>
// void CoutFormat(Fmt fmt, Args&&... args) {
//     std::cout << std::format(fmt, args...);
// }
#define ASSERT(expr, msg) assert((expr) && (msg))

