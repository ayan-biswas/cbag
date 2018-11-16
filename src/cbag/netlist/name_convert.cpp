/** \file netlist.h
 *  \brief This file defines classes that netlists objects.
 *
 *  \author Eric Chang
 *  \date   2018/07/10
 */

#include <cbag/netlist/name_convert.h>
#include <cbag/spirit/ast.h>
#include <cbag/spirit/name.h>
#include <cbag/spirit/name_unit.h>
#include <cbag/spirit/parsers.h>

namespace cbag {

spirit::ast::name_unit parse_cdba_name_unit(const std::string &source) {
    spirit::ast::name_unit ast;
    parse(source, spirit::name_unit(), ast);
    return ast;
}

spirit::ast::name parse_cdba_name(const std::string &source) {
    spirit::ast::name ast;
    parse(source, spirit::name(), ast);
    return ast;
}

} // namespace cbag
