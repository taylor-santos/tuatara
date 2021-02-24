#include "driver.h"

#include <fstream>

#include "scanner.h"

using namespace std;

namespace yy {

Driver::Driver()
    : lines{1}
    , scanner{make_unique<Scanner>()}
    , parser{make_unique<Parser>(*this)}
    , location{make_unique<yy::location>()}
    , output{cerr}
    , indentStack{{0}} {
    parser->set_debug_level(0);
    scanner->setDebug(false);
}

Driver::~Driver() = default;

int
Driver::parse(istream &in, ostream &out) {
    scanner->switch_streams(&in, &out);
    output = out;
    return parser->parse();
}

int
Driver::parseFile(const char *path) {
    ifstream s(path, ifstream::in);
    filename = path;
    location->initialize(&filename);
    scanner->switch_streams(&s, &cerr);
    output     = cerr;
    int result = parser->parse();

    s.close();

    return result;
}

bool
Driver::applyIndent(int indent) {
    int topIndent = indentStack.top();
    if (topIndent < indent) {
        tokenQ.emplace(yy::Parser::make_INDENT(*location));
        indentStack.push(indent);
        return false;
    }
    while (!indentStack.empty() && topIndent > indent) {
        indentStack.pop();
        tokenQ.emplace(yy::Parser::make_OUTDENT(*location));
        tokenQ.emplace(yy::Parser::make_NEWLINE(*location));
        topIndent = indentStack.top();
    }
    return topIndent < indent;
}

} // namespace yy
