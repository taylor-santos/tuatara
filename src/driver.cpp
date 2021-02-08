#include "driver.h"
#include "scanner.h"

namespace yy {

Driver::Driver()
    : scanner{std::make_unique<Scanner>()}
    , parser{std::make_unique<Parser>(*this)}
    , location{std::make_unique<yy::location>()}
    , output(std::cerr) {
    parser->set_debug_level(0);
    lines.emplace_back("");
}

Driver::~Driver() = default;

int
Driver::parse(std::istream &in, std::ostream &out) {
    scanner->switch_streams(&in, &out);
    output = out;
    return parser->parse();
}

int
Driver::parseFile(const char *path) {
    std::ifstream s(path, std::ifstream::in);
    std::string   filename(path);
    location->initialize(&filename);
    scanner->switch_streams(&s, &std::cerr);
    output = std::cerr;

    int result = parser->parse();

    s.close();

    return result;
}

} // namespace yy
