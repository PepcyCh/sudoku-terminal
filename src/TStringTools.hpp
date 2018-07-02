#pragma once

#ifndef T_STRING_TOOLS
#define T_STRING_TOOLS

#include <cctype>
#include <string>

namespace GameSudoku {

static std::string toLower(const std::string &str, int n) {
    std::string s = str.substr(0, n);
    for (int i = 0; i < n; i++) if (isalpha(s[i])) s[i] = tolower(s[i]);
    return s;
}

} // namespace GameSudoku

#endif // T_STRING_TOOLS
