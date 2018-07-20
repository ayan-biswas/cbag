/** \file cellviews.h
 *  \brief This file defines classes representing various cell views.
 *
 *  \author Eric Chang
 *  \date   2018/07/10
 */

#ifndef CBAG_DATABASE_CELLVIEWS_H
#define CBAG_DATABASE_CELLVIEWS_H

#include <map>

#include <cbag/database/datatypes.h>
#include <cbag/database/shapes.h>


namespace cbag {

    struct PinFigure;
    struct Instance;

    /** A schematic or symbol cell view
     *
     */
    struct SchCellView {
        SchCellView() = default;

        explicit SchCellView(const std::string &yaml_fname);

        // methods to manipulate parameters, so Cython doesn't have to worry about variants

        void clear_params();

        void set_int_param(const char *name, int value);

        void set_double_param(const char *name, double value);

        void set_bool_param(const char *name, bool value);

        void set_string_param(const char *name, const char *value);

        std::string lib_name, cell_name, view_name;
        std::map<std::string, PinFigure> in_terms, out_terms, io_terms;
        std::vector<Shape> shapes;
        std::map<std::string, Instance> instances;
        ParamMap props;
        ParamMap app_defs;
    };
}

#endif //CBAG_DATABASE_CELLVIEWS_H
