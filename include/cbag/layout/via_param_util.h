#ifndef CBAG_LAYOUT_VIA_PARAM_UTIL_H
#define CBAG_LAYOUT_VIA_PARAM_UTIL_H

#include <type_traits>

#include <cbag/common/transformation_fwd.h>
#include <cbag/enum/orient_2d.h>
#include <cbag/layout/via_param.h>

namespace cbag {

class box_t;

namespace layout {

bool empty(const via_param &p);

offset_t get_arr_dim(const via_param &p, orient_2d orient);

offset_t get_metal_dim(const via_param &p, orient_2d orient, cnt_t level);

box_t get_box(const via_param &p, const transformation &xform, cnt_t level);

std::array<offset_t, 2> get_via_extensions(const via_param &p, vector dim, orient_2d bot_dir,
                                           orient_2d top_dir);

} // namespace layout
} // namespace cbag

#endif
