#include "pch.h"
#include <regex>
#include <string>

class FEN {
public:
    static std::wstring to_wstring(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }


    static std::wstring FEN::GetPieces(std::string fen) {
        std::regex pattern(R"(([\w\d/]+)\s([wb])\s([KQkq\-]+)\s([\w\d\-]+)\s(\d+)\s(\d+))");

        std::smatch matches;

        if (std::regex_match(fen, matches, pattern)) {
            return to_wstring(matches[1].str());
        }
        else {
            return to_wstring("");
        }

    }
};
