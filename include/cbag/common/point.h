
#ifndef CBAG_COMMON_POINT_H
#define CBAG_COMMON_POINT_H

#if __has_include(<oa/oaDesignDB.h>)

#include <oa/oaDesignDB.h>

namespace cbag {

using point = oa::oaPoint;

} // namespace cbag

#else

#include <cbag/common/typedefs.h>

namespace cbag {

class point {
  private:
    coord_t xv = 0;
    coord_t yv = 0;

  public:
    point() {}

    point(coord_t xv, coord_t yv) : xv(xv), yv(yv) {}

    coord_t x() const { return xv; }

    coord_t y() const { return yv; }

    void set(coord_t x, coord_t y) {
        xv = x;
        yv = y;
    }
};

} // namespace cbag

#endif
#endif
