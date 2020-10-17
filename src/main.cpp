#include <iostream>

#include "driver.h"
#include "json.h"

using namespace std;

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filenames...>" << endl;
        return EXIT_FAILURE;
    }
    JSON::JSON::minimize = false;
    for (int i = 1; i < argc; i++) {
        yy::Driver drv;
        if (drv.parse_file(argv[i])) {
            cerr << argv[i] << ": failed to parse" << endl;
            break;
        }
        JSON::Array arr(cout);
        for (const auto &stmt : drv.statements) {
            arr.Next();
            cout << *stmt;
        }
    }
}
