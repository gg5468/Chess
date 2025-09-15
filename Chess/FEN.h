#include "pch.h"
#include <regex>
#include <string>

class fen {
public:
    static std::wstring to_wstring(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }

    static std::wstring GetPieces(const std::string& fen_str) {
        std::wstring wfen = to_wstring(fen_str);
        std::wregex pattern(L"([\\w\\d/]+)\\s([wb])\\s([KQkq\\-]+)\\s([\\w\\d\\-]+)\\s(\\d+)\\s(\\d+)");
        std::wsmatch matches;
        if (std::regex_match(wfen, matches, pattern)) {
            return matches[1].str();
        }
        return L"";
    }
};
