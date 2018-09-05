#ifndef CBAG_LAYOUT_CELLVIEW_H
#define CBAG_LAYOUT_CELLVIEW_H

#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#include <cbag/layout/geometry.h>
#include <cbag/layout/instance.h>
#include <cbag/layout/typedefs.h>

namespace cbag {
namespace layout {

class pt_vector;
class polygon;
class boundary;
class blockage;
class via;
class rectangle;
class polygon90;
class polygon45;
class polygon;
template <typename T> class polygon_ref;
class via_ref;

using layer_t = std::pair<lay_t, purp_t>;
using geo_map_t = std::unordered_map<layer_t, geometry, boost::hash<layer_t>>;
using block_map_t = std::unordered_map<layer_t, std::vector<blockage>, boost::hash<layer_t>>;
using inst_map_t = std::unordered_map<std::string, instance>;

class cellview {
  private:
    std::string tech;
    geo_map_t geo_map;
    inst_map_t inst_map;
    std::vector<via> via_list;
    block_map_t lay_block_map;
    std::vector<polygon> area_block_list;
    std::vector<boundary> boundary_list;
    uint32_t inst_name_cnt = 0;
    uint8_t geo_mode = 0;
    struct helper;

  public:
    explicit cellview(std::string tech, uint8_t geo_mode = 0);

    bool empty() const {
        return geo_map.empty() && inst_map.empty() && via_list.empty() && lay_block_map.empty() &&
               area_block_list.empty() && boundary_list.empty();
    }

    rectangle get_bbox(lay_t lay_id, purp_t purp_id) const;

    polygon_ref<rectangle> add_rect(lay_t lay_id, purp_t purp_id, coord_t xl, coord_t yl,
                                    coord_t xh, coord_t yh);

    polygon_ref<polygon90> add_poly90(lay_t lay_id, purp_t purp_id, pt_vector data);

    polygon_ref<polygon45> add_poly45(lay_t lay_id, purp_t purp_id, pt_vector data);

    polygon_ref<polygon> add_poly(lay_t lay_id, purp_t purp_id, pt_vector data);

    void add_path_seg(lay_t lay_id, purp_t purp_id, coord_t x0, coord_t y0, coord_t x1, coord_t y1,
                      dist_t width, const char *style0, const char *style1);

    inst_map_t::iterator add_prim_instance(const char *lib, const char *cell, const char *view,
                                           const char *name, transformation xform, uint32_t nx,
                                           uint32_t ny, offset_t spx, offset_t spy);

    inst_map_t::iterator add_instance(const cellview *cv, const char *name, transformation xform,
                                      uint32_t nx, uint32_t ny, offset_t spx, offset_t spy);

    via_ref add_via(transformation xform, const char *via_id, const uint32_t (&num)[2],
                    const dist_t (&cut_dim)[2], const offset_t (&cut_sp)[2],
                    const offset_t (&lay1_enc)[2], const offset_t (&lay1_off)[2],
                    const offset_t (&lay2_enc)[2], const offset_t (&lay2_off)[2], bool add_rect);
};

} // namespace layout
} // namespace cbag

#endif // CBAG_LAYOUT_GEOMETRIES_H
