#include <string>

#include <catch2/catch.hpp>

#include <fmt/core.h>

#include <cbag/cbag.h>
#include <cbag/oa/oa_database.h>

#include "util/io.h"

cbag::sch::cellview read_cv(const std::string &lib_name, const std::string &cell_name,
                            const std::string &view_name, const std::string &lib_file) {
    cbagoa::oa_database db(lib_file);
    return db.read_sch_cellview(lib_name, cell_name, view_name);
}

std::string write_cv(const cbag::sch::cellview &cv, const std::string &cell_name,
                     const std::string &view_name, const std::string &output_dir,
                     const std::string &expect_dir) {
    std::string out_fname, expect_fname;

    if (view_name == "schematic") {
        out_fname = fmt::format("{}/{}.yaml", output_dir, cell_name);
        expect_fname = fmt::format("{}/{}.yaml", expect_dir, cell_name);
    } else {
        out_fname = fmt::format("{}/{}.{}.yaml", output_dir, cell_name, view_name);
        expect_fname = fmt::format("{}/{}.{}.yaml", expect_dir, cell_name, view_name);
    }
    cv.to_file(out_fname);

    // check two file equivalent
    std::string output_str = read_file(out_fname);
    std::string expect_str = read_file(expect_fname);
    REQUIRE(output_str == expect_str);

    return out_fname;
}

SCENARIO("read/write OA data", "[oa]") {
    GIVEN("An OA library") {
        std::string lib_name = "test_netlist";
        std::string lib_file = "tests/data/cds.lib";
        std::string output_dir = "tests/data/test_outputs/yaml";
        std::string expect_dir = "tests/data/test_netlist_yaml";

        std::string cell_name = GENERATE(values<std::string>({
            "nmos4_standard",
            "pmos4_standard",
            "cv_simple",
            "cv_bus_term",
            "cv_inst_w_bus",
            "cv_array_inst_simple",
            "cv_array_inst_w_bus",
        }));
        std::string view_name = GENERATE(values<std::string>({"schematic", "symbol"}));

        THEN("cellview is read properly") {
            auto cv = read_cv(lib_name, cell_name, view_name, lib_file);
            AND_THEN("cellview written to yaml file properly") {
                auto out_fname = write_cv(cv, cell_name, view_name, output_dir, expect_dir);
                AND_THEN("can read cellview from yaml file") {
                    cbag::sch::cellview cv_test(out_fname);
                }
            }
        }
    }
}

TEST_CASE("Reading cds.lib with non-existent libray", "[oa]") {
    auto lib_file = "tests/data/cds_bad.lib";
    REQUIRE_THROWS_AS(cbagoa::oa_database(lib_file), std::runtime_error);
}
