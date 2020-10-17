#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "printer.h"

using namespace std;

void
Print::Error(const string &msg, const yy::location &location, const vector<string> &lines) {
    if (location.begin.filename) {
        cerr << *location.begin.filename << ":";
    }
    cerr << location.begin.line << ":" << location.begin.column << ": " << msg << endl;
    int num_width = to_string(location.end.line).length();
    for (auto i = location.begin.line; i <= location.end.line; i++) {
        string line = lines[i - 1];
        replace(line.begin(), line.end(), '\t', ' ');
        size_t first = line.find_first_not_of(' ');
        size_t last  = line.find_last_not_of(' ');
        if (first == string::npos) {
            continue;
        }
        cerr << right << setw(num_width) << i << " | ";
        cerr << line << endl;
        cerr << string(num_width, ' ') << " | ";
        auto first_col = i == location.begin.line ? location.begin.column - 1 : first;
        cerr << string(first_col, ' ');
        auto last_col = i == location.end.line ? location.end.column - 1 : last + 1;
        if (i == location.begin.line) {
            cerr << "^";
            last_col--;
        }
        if (last_col > first_col) {
            cerr << string(last_col - first_col, '~');
        }
        cerr << endl;
    }
}
