#include "printer.h"

#include <algorithm>
#include <iomanip>

#include "location.hh"

using namespace std;

namespace Print {

void
error(ostream &out, const string &msg, const yy::location &location, const vector<string> &lines) {
    if (location.begin.line == 0) {
        // Some errors will originate outside of source code, just print the message in that case.
        out << msg << endl;
        return;
    }
    size_t numWidth = to_string(location.end.line).length();
    out << "*" << std::string(numWidth, ' ');
    if (location.begin.filename) {
        out << *location.begin.filename << ":";
    }
    out << location.begin.line << ":" << location.begin.column << " - " << location.end.line << ":"
        << location.end.column << ": " << msg << endl;
    for (auto i = location.begin.line; i <= location.end.line; i++) {
        string line = pretty(lines[i - 1]);
        std::replace(line.begin(), line.end(), '\t', ' ');
        size_t first = line.find_first_not_of(' ');
        size_t last  = line.find_last_not_of(' ');
        if (first == string::npos) {
            continue;
        }
        out << right << std::setw(numWidth) << i << " | ";
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

std::string
pretty(string str) {
    std::replace(str.begin(), str.end(), '\t', ' ');
    str.erase(
        std::remove_if(str.begin(), str.end(), [](unsigned char c) { return !std::isprint(c); }),
        str.end());
    return str;
}

} // namespace Print
