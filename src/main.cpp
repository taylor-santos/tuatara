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
        if (drv.parseFile(argv[i])) {
            cerr << argv[i] << ": failed to parse" << endl;
            break;
        } else {
            JSON::Array(cout) << drv.statements;
        }
    }
}
