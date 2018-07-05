//
// Created by erichang on 7/5/18.
//

#ifndef CBAG_SPIRIT_RANGE_DEF_H
#define CBAG_SPIRIT_RANGE_DEF_H

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include <cbag/spirit/ast.h>
#include <cbag/spirit/ast_adapted.h>
#include <cbag/spirit/error_handler.h>
#include <cbag/spirit/range.h>

namespace cbag {
    namespace spirit {
        namespace parser {
            namespace x3 = boost::spirit::x3;

            auto set_stop = [](auto &ctx) {
                x3::_val(ctx).stop = x3::_attr(ctx);
            };

            auto check_zero = [](auto &ctx) {
                x3::_pass(ctx) = (x3::_attr(ctx) != 0);
            };

            range_type const range = "range";

            /** Grammer for range
             *
             *  range is of the form <a:b:c>, b and c are optional.
             *  if b is missing, then b = a by default.  If c is missing, then c = 1 (initialized by struct).
             *  c cannot be 0.
             */
            auto const range_def =
                    x3::rule<range_class, ast::range, true>{}
                            = '<' > (x3::uint32[set_stop])
                            >> -(':' > x3::uint32 >> -(':' > (x3::uint32[check_zero]))) > '>';

            BOOST_SPIRIT_DEFINE(range);

            struct range_class : x3::annotate_on_success, error_handler_base {
            };

        }

    }
}
#endif //CBAG_SPIRIT_RANGE_DEF_H
