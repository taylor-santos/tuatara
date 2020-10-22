#ifndef PRINTER_H
#define PRINTER_H

#include <memory>
#include <vector>

#include "location.hh"

namespace Print {

void Error(
    std::ostream &                  out,
    const std::string &             msg,
    const yy::location &            location,
    const std::vector<std::string> &lines);

}

#endif // PRINTER_H
