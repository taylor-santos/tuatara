#include <iomanip>

#include "type/type_context.h"
#include "type/type_exception.h"

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
            TypeChecker::Context ctx;
            try {
                for (const auto &stmt : drv.statements) {
                    stmt->getType(ctx);
                    cout << *stmt << endl;
                    stmt->walk([&](const AST::Node &node) -> void {
                        Print::error(cout, node.getNodeName(), node.getLoc(), drv.lines);
                    });
                    int maxLen = max_element(
                                     ctx.getSymbols().begin(),
                                     ctx.getSymbols().end(),
                                     [](const auto &s1, const auto &s2) -> bool {
                                         return s1.second.name.length() < s2.second.name.length();
                                     })
                                     ->second.name.length();
                    cout << setw(maxLen) << left << "Name"
                         << " | "
                         << "Type" << endl;
                    for (const auto &[name, symbol] : ctx.getSymbols()) {
                        cout << setw(maxLen) << left << name << " : ";
                        symbol.type.pretty(cout);
                        cout << endl;
                    }
                }
            } catch (TypeChecker::TypeException &e) {
                for (const auto &[msg, loc] : e.getMsgs()) {
                    Print::error(cerr, msg, loc, drv.lines);
                }
            }
            // JSON::Object json(cout);
            // json.printKeyValue("statements", drv.statements);
            // cout << endl;
        }
    }
}
