#include <cbag/common/box_t_util.h>
#include <cbag/common/vector.h>
#include <cbag/layout/via.h>

namespace cbag {
namespace layout {

struct via::helper {
    static box_t get_box(const via &self, const vector &offset, const vector &enc) {
        auto nx = self.params.num[0];
        auto ny = self.params.num[1];
        auto via_w = nx * self.params.cut_dim[0] + (nx - 1) * self.params.cut_spacing.first;
        auto via_h = ny * self.params.cut_dim[1] + (ny - 1) * self.params.cut_spacing.second;

        auto xl = static_cast<coord_t>(offset.first - (via_w / 2) - enc.first);
        auto yl = static_cast<coord_t>(offset.second - (via_h / 2) - enc.second);
        auto xh = static_cast<coord_t>(xl + via_w + enc.first);
        auto yh = static_cast<coord_t>(yl + via_h + enc.second);

        box_t r{xl, yl, xh, yh};
        transform(r, self.xform);
        return r;
    }
};

via::via() = default;

via::via(cbag::transformation xform, std::string via_id, via_param params, bool add_layers,
         bool bot_horiz, bool top_horiz)
    : xform(std::move(xform)), via_id(std::move(via_id)), params(std::move(params)),
      add_layers(add_layers), bot_horiz(bot_horiz), top_horiz(top_horiz) {}

box_t via::bot_box() const { return helper::get_box(*this, params.lay1_off, params.lay1_enc); }

box_t via::top_box() const { return helper::get_box(*this, params.lay2_off, params.lay2_enc); }

} // namespace layout
} // namespace cbag
