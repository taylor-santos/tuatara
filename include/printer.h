#ifndef PRINTER_H
#define PRINTER_H

#include <memory>
#include <vector>

#include "location.hh"

namespace Print {

void error(
    std::ostream &                  out,
    const std::string &             msg,
    const yy::location &            location,
    const std::vector<std::string> &lines);

std::string pretty(std::string str);

} // namespace Print

#endif // PRINTER_H
