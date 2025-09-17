#include "pch.h"
#include <regex>
#include <string>

class fen {
public:
    static std::wstring GetPieces(const std::wstring& fen_str) {
        std::wregex pattern(L"([\\w\\d/]+)\\s([wb])\\s([KQkq\\-]+)\\s([\\w\\d\\-]+)\\s(\\d+)\\s(\\d+)");
        std::wsmatch matches;
        if (std::regex_match(fen_str, matches, pattern)) {
            return matches[1].str();
        }
        return L"";
    }
};
