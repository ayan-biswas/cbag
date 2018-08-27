/** \file shapes.h
 *  \brief This file defines various shapes used by the database.
 *
 *  \author Eric Chang
 *  \date   2018/07/19
 */
#ifndef CBAG_SCHEMATIC_SHAPE_BASE_H
#define CBAG_SCHEMATIC_SHAPE_BASE_H

#include <string>

#include <cbag/common/typedefs.h>

namespace cbag {
namespace sch {

struct shape_base {
    inline shape_base() : layer(0), purpose(0), net() {}

    inline shape_base(lay_t lay, purp_t purp, std::string net)
        : layer(lay), purpose(purp), net(std::move(net)) {}

    lay_t layer;
    purp_t purpose;
    std::string net;
};

} // namespace sch
} // namespace cbag

#endif
