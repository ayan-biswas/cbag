#include <cbag/layout/via_param.h>

namespace cbag {
namespace layout {

void set_via_enc_offset(vector &enc, vector &off, offset_t encl, offset_t encr, offset_t enct,
                        offset_t encb) {
    enc[0] = (encr + encl) / 2;
    enc[1] = (enct + encb) / 2;
    off[0] = encr - enc[0];
    off[1] = enct - enc[1];
}

via_param::via_param() = default;

via_param::via_param(cnt_t vnx, cnt_t vny, offset_t w, offset_t h, offset_t vspx, offset_t vspy,
                     offset_t enc1l, offset_t enc1r, offset_t enc1t, offset_t enc1b, offset_t enc2l,
                     offset_t enc2r, offset_t enc2t, offset_t enc2b)
    : num{vnx, vny}, cut_dim{{w, h}}, cut_spacing{vspx, vspy} {
    set_via_enc_offset(enc[0], off[0], enc1l, enc1r, enc1t, enc1b);
    set_via_enc_offset(enc[1], off[1], enc2l, enc2r, enc2t, enc2b);
}

bool via_param::operator==(const via_param &rhs) const noexcept {
    return num == rhs.num && cut_dim == rhs.cut_dim && cut_spacing == rhs.cut_spacing &&
           enc == rhs.enc && off == rhs.off;
}

} // namespace layout
} // namespace cbag
