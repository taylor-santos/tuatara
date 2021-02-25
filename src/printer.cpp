#include "printer.h"

#include <algorithm>
#include <iomanip>

#include "location.hh"

using std::endl, std::ostream, std::right, std::setw, std::string, std::to_string, std::vector;

namespace Print {

void
error(ostream &out, const string &msg, const yy::location &location, const vector<string> &lines) {
    if (location.begin.line == 0) {
        // Some errors will originate outside of source code, and will therefore have an invalid
        // location. Print only the message in this case.
        out << msg << endl;
        return;
    }
    size_t numWidth = to_string(location.end.line).length();
    out << "*" << string(numWidth, ' ');
    if (location.begin.filename) {
        out << *location.begin.filename << ":";
    }
    out << location.begin.line << ":" << location.begin.column << " - " << location.end.line << ":"
        << location.end.column << ": " << msg << endl;
    // If the last line ends before the first character, skip it.
    auto endLine = location.end.column > 1 ? location.end.line : location.end.line - 1;
    for (auto i = location.begin.line; i <= endLine; i++) {
        string line  = pretty(lines[i - 1]);
        size_t first = line.find_first_not_of(' ');
        size_t last  = line.find_last_not_of(' ');
        if (first == string::npos) {
            continue;
        }
        out << right << setw(numWidth) << i << " | ";
        out << line << endl;
        out << string(numWidth, ' ') << " | ";
        auto firstCol = i == location.begin.line ? location.begin.column - 1 : first;
        out << string(firstCol, ' ');
        auto lastCol = i == location.end.line ? location.end.column - 1 : last + 1;
        if (i == location.begin.line) {
            out << "^";
            if (lastCol > 0) lastCol--;
        }
        if (lastCol > firstCol) {
            out << string(lastCol - firstCol, '~');
        }
        out << endl;
    }
}

string
pretty(string str) {
    replace(str.begin(), str.end(), '\t', ' ');
    str.erase(
        remove_if(str.begin(), str.end(), [](unsigned char c) { return c == '\n' || c == '\r'; }),
        str.end());
    return str;
}

} // namespace Print
