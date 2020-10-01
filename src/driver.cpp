#include "driver.h"
#include "parser.h"

Driver::Driver()
    : trace_parsing(false)
    , trace_scanning(false) {}

int
Driver::parse(const std::string &f) {
    file = f;
    location.initialize(&file);
    scan_begin();

    yy::parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_end();
    return res;
}