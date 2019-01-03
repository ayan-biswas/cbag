#include <cbag/layout/pin.h>

namespace cbag {
namespace layout {

pin::pin() = default;

pin::pin(coord_t xl, coord_t yl, coord_t xh, coord_t yh, std::string net, std::string label)
    : box_t(xl, yl, xh, yh), net(std::move(net)), label(std::move(label)) {}

pin::pin(box_t box, std::string net, std::string label)
    : box_t(std::move(box)), net(std::move(net)), label(std::move(label)) {}

bool pin::operator==(const pin &rhs) const noexcept {
    return box_t::operator==(rhs) && net == rhs.net && label == rhs.label;
}

} // namespace layout
} // namespace cbag
