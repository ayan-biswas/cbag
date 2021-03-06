#ifndef CBAG_LAYOUT_GEO_OBJECT_H
#define CBAG_LAYOUT_GEO_OBJECT_H

#include <variant>

#include <boost/geometry.hpp>
#include <boost/polygon/polygon.hpp>

#include <cbag/common/box_t.h>
#include <cbag/common/typedefs.h>
#include <cbag/layout/geo_instance.h>
#include <cbag/layout/polygon.h>
#include <cbag/layout/polygon45_fwd.h>
#include <cbag/layout/polygon90_fwd.h>

namespace bg = boost::geometry;

namespace cbag {
namespace layout {

using bg_point = bg::model::point<coord_t, 2, bg::cs::cartesian>;
using bg_box = bg::model::box<bg_point>;

class geo_object {
  public:
    using value_type = std::variant<box_t, polygon90, polygon45, polygon, geo_instance>;
    using box_type = bg_box;

  public:
    value_type val;
    offset_t spx = 0;
    offset_t spy = 0;
    box_type bnd_box;

    template <typename T> geo_object(T &&v, offset_t spx, offset_t spy);

    bool operator==(const geo_object &v) const;

    const geo_instance *get_instance() const;
};

bg_box get_bnd_box(const geo_object::value_type &val, offset_t spx, offset_t spy);

template <typename T>
geo_object::geo_object(T &&v, offset_t spx, offset_t spy)
    : val(std::forward<T>(v)), spx(spx), spy(spy), bnd_box(get_bnd_box(val, spx, spy)) {}

} // namespace layout
} // namespace cbag

namespace boost {
namespace geometry {
namespace index {

template <> struct indexable<cbag::layout::geo_object> {
    using result_type = const cbag::layout::geo_object::box_type &;

    result_type operator()(const cbag::layout::geo_object &v) const { return v.bnd_box; }
};

} // namespace index
} // namespace geometry
} // namespace boost

#endif
