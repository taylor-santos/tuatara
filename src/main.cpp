#include <iostream>

#include "driver.h"

using namespace std;

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filenames...>" << endl;
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++) {
        Driver drv;
        if (drv.parse(argv[i])) {
            cerr << argv[i] << ": failed to parse" << endl;
            break;
        }
    }
}
