#include <fstream>
#include <sstream>

#include <boost/archive/xml_oarchive.hpp>

#include <cbagoa/database.h>


int read_oa() {
    std::string lib_file("cds.lib");
    std::string lib_path(".");
    std::string tech_lib("cds_ff_mpt");
    std::string lib_name("schtest");
    std::string cell_name("inv");
    std::string cell_name2("inv2");
    std::string view_name("schematic");
    std::string sym_view_name("symbol");

    cbagoa::OADatabase db(lib_file);

    cbag::SchCellView sch_master = db.read_sch_cellview(lib_name, cell_name, view_name);

    std::ofstream outfile;
    outfile.open("inv_sch.xml", std::ios_base::out);
    auto xml_out = std::make_unique<boost::archive::xml_oarchive>(outfile);
    (*xml_out) << boost::serialization::make_nvp("master", sch_master);
    xml_out.reset();
    outfile.close();

    /*
    cbag::SchCellView sym_master = db.read_sch_cellview(lib_name, cell_name, sym_view_name);

    std::ofstream symfile("inv_sym.xml", std::ios_base::out);
    xml_out = std::make_unique<boost::archive::xml_oarchive>(symfile);
    (*xml_out) << boost::serialization::make_nvp("master", sym_master);
    xml_out.reset();
    symfile.close();
    */

    /*
    db.write_sch_cellview(lib_name, cell_name2, view_name, true, sch_master);

    sch_master = db.read_sch_cellview(lib_name, cell_name2, view_name);

    outfile.open("inv_sch2.xml", std::ios_base::out);
    xml_out = std::make_unique<boost::archive::xml_oarchive>(outfile);
    (*xml_out) << boost::serialization::make_nvp("master", sch_master);
    xml_out.reset();
    outfile.close();
    */
    return 0;
}

int main(int argc, char *argv[]) {
    return read_oa();
}
