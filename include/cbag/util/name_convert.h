/** \file netlist.h
 *  \brief This file defines classes that netlists objects.
 *
 *  \author Eric Chang
 *  \date   2018/07/10
 */

#ifndef CBAG_UTIL_NAME_CONVERT_H
#define CBAG_UTIL_NAME_CONVERT_H

#include <string>

namespace cbag {

namespace spirit {
namespace ast {
struct name_unit;
struct name;
struct name_rep;
struct namespace_info;
} // namespace ast
} // namespace spirit

// string -> name conversion methods

namespace util {

spirit::ast::name_unit parse_cdba_name_unit(const std::string &source);

spirit::ast::name parse_cdba_name(const std::string &source);

spirit::ast::name_rep parse_cdba_name_rep(const std::string &source);

} // namespace util
} // namespace cbag

#endif // CBAG_NETLIST_NAME_CONVERT_H
