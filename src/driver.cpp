#include "driver.h"
#include "parser.tab.hh"
#include "scanner.h"
#include "location.hh"

#include <memory>

namespace yy {

Driver::Driver()
    : scanner{std::make_unique<Scanner>()}
    , parser{std::make_unique<Parser>(*this)}
    , location{std::make_unique<yy::location>()} {
    parser->set_debug_level(0);
    lines.emplace_back("");
}

Driver::~Driver() {}

void
Driver::reset() {
    location = std::make_unique<yy::location>();
}

int
Driver::parse(std::istream &in, std::ostream &out) {
    scanner->switch_streams(&in, &out);
    return parser->parse();
}

int
Driver::parse_file(const char *path) {
    std::ifstream s(path, std::ifstream::in);
    std::string   filename(path);
    location->initialize(&filename);
    scanner->switch_streams(&s, &std::cerr);

    int result = parser->parse();

    s.close();

    return result;
}

} // namespace yy
