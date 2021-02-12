#include <iostream>

#include "driver.h"
#include "json.h"
#include "printer.h"

using namespace std;

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filenames...>" << endl;
        return EXIT_FAILURE;
    }
    JSON::JSON::setMinimize(false);
    for (int i = 1; i < argc; i++) {
        yy::Driver drv;
        if (drv.parseFile(argv[i])) {
                cerr << argv[i] << ": failed to parse" << endl;
            break;
        } else {
            for (const auto &stmt : drv.statements) {
                stmt->walk([&](const AST::Node &node) -> void {
                    Print::error(cout, node.getTypeName(), node.getLoc(), drv.lines);
                });
            }
            JSON::Object json(cout);
            json.printKeyValue("statements", drv.statements);
            cout << endl;
        }
    }
}
