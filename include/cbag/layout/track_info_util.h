#ifndef CBAG_LAYOUT_TRACK_INFO_UTIL_H
#define CBAG_LAYOUT_TRACK_INFO_UTIL_H

#include <cbag/common/transformation_fwd.h>
#include <cbag/enum/round_mode.h>
#include <cbag/layout/track_info.h>

namespace cbag {
namespace layout {

htr_t coord_to_htr(offset_t coord, offset_t pitch, offset_t off, round_mode mode, bool even);

htr_t coord_to_htr(const track_info &tr_info, offset_t coord, round_mode mode = round_mode::NONE,
                   bool even = false);

htr_t find_next_htr(const track_info &tr_info, offset_t coord, cnt_t ntr,
                    round_mode mode = round_mode::NONE, bool even = false);

offset_t htr_to_coord(const track_info &tr_info, htr_t htr) noexcept;

cnt_t get_min_space_htr(const track_info &tr_info, const tech &t, level_t level, cnt_t num_tr,
                        bool same_color, bool even);

// TODO: change definition of spacing to get more optimized layout
htr_t transform_htr(const track_info &tr_info, htr_t htr, const transformation &xform);

} // namespace layout
} // namespace cbag

#endif
