
#ifndef CBAG_COMMON_BOX_T_H
#define CBAG_COMMON_BOX_T_H

#include <array>

#include <cbag/common/typedefs.h>

namespace cbag {

struct box_t {
  public:
    std::array<coord_t, 2> intvs[2];

    box_t() noexcept;
    box_t(coord_t xl, coord_t yl, coord_t xh, coord_t yh) noexcept;
    box_t(uint8_t orient_code, coord_t tl, coord_t th, coord_t pl, coord_t ph) noexcept;
};

} // namespace cbag

#endif
