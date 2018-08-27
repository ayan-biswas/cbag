/** \file cellviews.h
 *  \brief This file defines classes representing various cell views.
 *
 *  \author Eric Chang
 *  \date   2018/07/10
 */

#ifndef CBAG_SCHEMATIC_CELLVIEWS_H
#define CBAG_SCHEMATIC_CELLVIEWS_H

#include <map>
#include <memory>
#include <unordered_map>

#include <cbag/common/datatypes.h>
#include <cbag/common/primitives.h>
#include <cbag/schematic/shapes_fwd.h>

namespace cbag {

struct PinFigure;
struct SchInstance;

using conn_list_t = std::vector<std::pair<std::string, std::string>>;
using inst_iter_t = std::map<std::string, SchInstance>::iterator;

/** A simple struct representing netlist information of a cellview.
 */
struct SchCellViewInfo {
    inline SchCellViewInfo()
        : cell_name(), in_terms(), out_terms(), io_terms(), props(), is_prim(false) {}

    inline SchCellViewInfo(std::string name, size_t num_in, size_t num_out, size_t num_inout,
                           bool is_prim)
        : cell_name(name), is_prim(is_prim) {
        in_terms.reserve(num_in);
        out_terms.reserve(num_out);
        io_terms.reserve(num_inout);
    }

    std::string cell_name;
    std::vector<std::string> in_terms, out_terms, io_terms;
    ParamMap props;
    bool is_prim;
};

/** A schematic or symbol cell view
 */
struct SchCellView {
    SchCellView() = default;

    explicit SchCellView(const char *yaml_fname, const char *sym_view = nullptr);

    inline SchCellView(const char *lib_name, const char *cell_name, const char *view_name,
                       box_t bbox)
        : lib_name(lib_name), cell_name(cell_name), view_name(view_name), bbox(std::move(bbox)) {}

    // methods to manipulate parameters, so Cython doesn't have to worry about
    // variants

    void clear_params();

    void set_int_param(const char *name, int value);

    void set_double_param(const char *name, double value);

    void set_bool_param(const char *name, bool value);

    void set_string_param(const char *name, const char *value);

    void rename_pin(const char *old_name, const char *new_name);

    void add_pin(const char *new_name, uint32_t term_type);

    bool remove_pin(const char *name);

    void rename_instance(const char *old_name, const char *new_name);

    bool remove_instance(const char *name);

    inst_iter_t copy_instance(const SchInstance &inst, uint32_t old_size,
                              const std::string &new_name, coord_t dx, coord_t dy,
                              const conn_list_t &conns);

    std::vector<inst_iter_t> array_instance(const char *old_name,
                                            const std::vector<std::string> &name_list, coord_t dx,
                                            coord_t dy, const std::vector<conn_list_t> &conns_list);

    SchCellViewInfo get_info(const std::string &cell_name) const;

    std::string lib_name, cell_name, view_name;
    box_t bbox;
    std::map<std::string, PinFigure> in_terms, out_terms, io_terms;
    std::vector<Shape> shapes;
    std::map<std::string, SchInstance> instances;
    ParamMap props;
    ParamMap app_defs;
    std::unique_ptr<SchCellView> sym_ptr;
};

// netlist map typedefs and functions

using lib_map_t = std::unordered_map<std::string, SchCellViewInfo>;
using netlist_map_t = std::unordered_map<std::string, lib_map_t>;

netlist_map_t load_netlist_map(const char *fname);

} // namespace cbag

#endif // CBAG_SCHEMATIC_CELLVIEWS_H
