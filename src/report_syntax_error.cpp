#include <iomanip>
#include <sstream>

#include "driver.h"
#include "grammar.h"
#include "parser.tab.hh"
#include "printer.h"

using std::setw;

namespace yy {

void
Parser::report_syntax_error(yy::Parser::context const &ctx) const {
    const auto &lookahead = ctx.lookahead();
    auto        location  = ctx.location();
    int         num       = ctx.expected_tokens(nullptr, 100);
    std::string sep;

    /*
    for (auto s = yystack_.begin(); s != yystack_.end(); s++) {
        auto &state = states[s->state];
        std::vector<std::tuple<int, std::string, std::string, std::string, bool>> lines;
        size_t maxLen = 0, ntLen = 0;
        for (auto &[r, point, transition, lah] : state.rules) {
            auto &      rule = rules[r];
            std::string before, after;
            sep = " ";
            for (int i = 0; i < point; i++) {
                before += sep + rule.rhs[i];
                sep = " ";
            }
            maxLen = std::max(maxLen, before.length());
            ntLen  = std::max(ntLen, rule.lhs.length());
            sep    = " ";
            for (size_t i = point; i < rule.rhs.size(); i++) {
                after += sep + rule.rhs[i];
                sep = " ";
            }

            if (!lah.empty()) {
                sep = " [";
                for (auto &l : lah) {
                    after += sep + l;
                    sep = ", ";
                }
                after += "]";
            }
            bool accepted = s + 1 != yystack_.end() && (s + 1)->state == transition;
            lines.emplace_back(r, rule.lhs, before, after, accepted);
        }
        driver.output.get() << "State " << state.number << ":" << std::endl;
        auto lastLine = lines.back();
        auto lastNum  = std::get<0>(lastLine);
        auto numLen   = std::to_string(lastNum).length();
        for (auto &[r, nt, before, after, accepted] : lines) {
            std::string start = accepted ? "[" : " ";
            driver.output.get() << start << std::setw(numLen) << std::right << r << " ";
            driver.output.get() << std::setw(ntLen) << std::left << nt << " :";
            driver.output.get() << std::setw(maxLen) << std::right << before << " ." << after;
            driver.output.get() << std::endl;
        }
    }
    */
    std::stringstream ss;
    ss << "syntax error: unexpected " << lookahead.name();
    Print::error(driver.output, ss.str(), location, driver.lines);
    std::unique_ptr<symbol_kind_type[]> expected(new symbol_kind_type[num]);
    ctx.expected_tokens(expected.get(), num);
    sep = "expected: ";
    std::string end;
    for (int i = 0; i < num - 1; i++) {
        driver.output.get() << sep << "\"" << symbol_name(expected[i]) << "\"";
        sep = ", ";
        end = "or ";
    }
    driver.output.get() << sep << end << "\"" << symbol_name(expected[num - 1]) << "\""
                        << std::endl;
}

} // namespace yy
