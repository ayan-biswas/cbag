#include <algorithm>

#include <boost/functional/hash.hpp>

#include <cbag/layout/via_info.h>
#include <cbag/util/unique_heap.h>

namespace cbag {
namespace layout {

constexpr offset_t MAX_SP = std::numeric_limits<offset_t>::max();

venc_data::venc_data() = default;

via_info::via_info() = default;

via_info::via_info(std::string &&ctype, vector &&cdim, vector &&s, std::vector<vector> &&s2_list,
                   std::vector<vector> &&s3_list, std::array<venc_info, 2> &&e_list)
    : cut_type(std::move(ctype)), cut_dim(std::move(cdim)), sp(std::move(s)),
      sp2_list(std::move(s2_list)), sp3_list(std::move(s3_list)), enc_list(std::move(e_list)) {
    if (sp2_list.empty()) {
        sp2_list.emplace_back(sp);
    }
    if (sp3_list.empty()) {
        sp3_list.insert(sp3_list.end(), sp2_list.begin(), sp2_list.end());
    }
}

cnt_t get_n_max(offset_t dim, offset_t w, offset_t sp) {
    if (sp == MAX_SP) {
        return (dim > w) ? 1 : 0;
    }
    return (dim + sp) / (w + sp);
}

offset_t get_arr_dim(cnt_t n, offset_t w, offset_t sp) {
    if (sp == MAX_SP)
        if (n != 1)
            return 0;
        else
            return w;
    else
        return n * (w + sp) - sp;
}

const std::vector<vector> &get_enc_list(const venc_info &enc_info, offset_t width) {
    for (const auto &data : enc_info) {
        if (width <= data.width)
            return data.enc_list;
    }
    return enc_info.back().enc_list;
}

offset_t get_enclosure_dim(vector box_dim, vector arr_dim, const venc_info &enc_info, orient_2d dir,
                           bool extend) {
    // TODO: implement this
    return 0;
}

via_cnt_t via_info::get_num_via(vector box_dim, orient_2d bot_dir, orient_2d top_dir,
                                bool extend) const {
    // get maximum possible number of vias
    vector min_sp = sp;
    for (const auto &arr : sp2_list) {
        min_sp[0] = std::min(min_sp[0], arr[0]);
        min_sp[1] = std::min(min_sp[1], arr[1]);
    }
    for (const auto &arr : sp3_list) {
        min_sp[0] = std::min(min_sp[0], arr[0]);
        min_sp[1] = std::min(min_sp[1], arr[1]);
    }
    auto nx_max = get_n_max(box_dim[0], cut_dim[0], min_sp[0]);
    auto ny_max = get_n_max(box_dim[1], cut_dim[1], min_sp[1]);

    // check if it's possible to palce a via
    if (nx_max == 0 || ny_max == 0)
        return {0, std::array<cnt_t, 2>{0, 0}};

    // use priority queue to find maximum number of vias
    util::unique_heap<via_cnt_t, boost::hash<via_cnt_t>> heap;
    heap.emplace(nx_max * ny_max, std::array<cnt_t, 2>{nx_max, ny_max});

    std::vector<vector> sp1_list({sp});

    while (!heap.empty()) {
        auto &[via_cnt, num_via] = heap.top();

        // get list of valid via spacing
        const std::vector<vector> *sp_vec_ptr;
        if (num_via[0] == 2 && num_via[1] == 2) {
            sp_vec_ptr = &sp2_list;
        } else if (num_via[0] > 1 && num_via[1] > 1) {
            sp_vec_ptr = &sp3_list;
        } else {
            sp_vec_ptr = &sp1_list;
        }

        // find optimal legal enclosure via
        vector opt_enc_dim = {{MAX_SP, MAX_SP}};
        const vector *opt_sp = nullptr;
        for (const auto &sp_via : *sp_vec_ptr) {
            vector arr_dim = {{get_arr_dim(num_via[0], cut_dim[0], sp_via[0]),
                               get_arr_dim(num_via[1], cut_dim[1], sp_via[1])}};
            if (arr_dim[0] == 0 || arr_dim[1] == 0)
                continue;

            auto bot_dim = get_enclosure_dim(box_dim, arr_dim, enc_list[0], bot_dir, extend);
            if (bot_dim == 0)
                continue;
            auto top_dim = get_enclosure_dim(box_dim, arr_dim, enc_list[1], top_dir, extend);
            if (top_dim == 0)
                continue;

            if (bot_dim <= opt_enc_dim[0] && top_dim <= opt_enc_dim[1]) {
                opt_enc_dim[0] = bot_dim;
                opt_enc_dim[1] = top_dim;
                opt_sp = &sp_via;
            }
        }

        if (opt_sp == nullptr) {
            // fail to find legal via enclosure
            // continue searching
            if (nx_max > 1)
                heap.emplace((nx_max - 1) * ny_max, std::array<cnt_t, 2>{nx_max - 1, ny_max});
            if (ny_max > 1)
                heap.emplace(nx_max * (ny_max - 1), std::array<cnt_t, 2>{nx_max, ny_max - 1});
        } else {
            // solution found
            // TODO: return best space and enclosure
            return heap.top();
        }

        heap.pop();
    }

    return {0, std::array<cnt_t, 2>{0, 0}};
}

} // namespace layout
} // namespace cbag