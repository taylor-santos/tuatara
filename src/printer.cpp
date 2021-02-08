#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "printer.h"

using namespace std;

void
Print::Error(
    ostream &             out,
    const string &        msg,
    const yy::location &  location,
    const vector<string> &lines) {
    if (location.begin.filename) {
        out << *location.begin.filename << ":";
    }
    out << location.begin.line << ":" << location.begin.column << ": " << msg << endl;
    size_t numWidth = to_string(location.end.line).length();
    for (auto i = location.begin.line; i <= location.end.line; i++) {
        string line = lines[i - 1];
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
            lastCol--;
        }
        if (lastCol > firstCol) {
            out << string(lastCol - firstCol, '~');
        }
        out << endl;
    }
}
