/** \file read_oa.h
 *  \brief This file converts OpenAccess objects to CBAG data structure.
 *
 *  \author Eric Chang
 *  \date   2018/07/13
 */

#ifndef CBAGOA_READ_OA_H
#define CBAGOA_READ_OA_H

#include <easylogging++.h>

 #include <oa/oaDesignDB.h>

#include <cbag/spirit/ast.h>
#include <cbag/database/figures.h>
#include <cbag/database/cellviews.h>


namespace bsa = cbag::spirit::ast;

namespace cbagoa {

    class OAReader {
    public:

        OAReader(oa::oaCdbaNS ns, el::Logger *logger)
                : ns(std::move(ns)), logger(logger) {};

        // String parsing methinds

        bsa::name parse_name(const oa::oaString &source);

        bsa::name_unit parse_name_unit(const oa::oaString &source);

        // Read method for properties

        std::pair<std::string, cbag::value_t> read_prop(oa::oaProp *prop_ptr);

        // Read methods for shapes

        cbag::Rect read_rect(oa::oaRect *p);

        cbag::Poly read_poly(oa::oaPolygon *p);

        cbag::Arc read_arc(oa::oaArc *p);

        cbag::Donut read_donut(oa::oaDonut *p);

        cbag::Ellipse read_ellipse(oa::oaEllipse *p);

        cbag::Line read_line(oa::oaLine *p);

        cbag::Path read_path(oa::oaPath *p);

        cbag::Text read_text(oa::oaText *p);

        cbag::EvalText read_eval_text(oa::oaEvalText *p);

        cbag::Shape read_shape(oa::oaShape *p);

        // Read method for references

        cbag::Instance read_instance(oa::oaInst *p);

        std::pair<bsa::name_unit, cbag::Instance> read_instance_pair(oa::oaInst *p);

        // Read method for pin figures

        cbag::PinFigure read_pin_figure(oa::oaTerm *t, oa::oaPinFig *p);

        // Read method for terminals

        std::pair<bsa::name, cbag::PinFigure> read_terminal_single(oa::oaTerm *term);

        // Read method for schematic/symbol cell view

        cbag::SchCellView read_sch_cellview(oa::oaDesign *design);

    private:
        const oa::oaCdbaNS ns;
        el::Logger *logger;
    };
}

#endif //CBAGOA_READ_OA_H
