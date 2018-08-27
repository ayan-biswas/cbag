/** \file netlist.h
 *  \brief This file defines classes that netlists objects.
 *
 *  \author Eric Chang
 *  \date   2018/07/10
 */

#ifndef CBAG_NETLIST_NETLIST_H
#define CBAG_NETLIST_NETLIST_H

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <cbag/netlist/name_convert.h>
#include <cbag/schematic/cellviews_fwd.h>

namespace cbag {

// netlister base class

class NetlistBuilder {
  public:
    /** A helper class for writing a line with column limit
     */
    class LineBuilder {
      public:
        LineBuilder(size_t ncol, char cnt_char, bool break_before, int tab_size);

        friend LineBuilder &operator<<(LineBuilder &builder, const std::string &token);

        friend LineBuilder &operator<<(LineBuilder &builder, std::string &&token);

        friend std::ofstream &operator<<(std::ofstream &stream, const LineBuilder &b);

      private:
        std::vector<std::string> tokens;
        size_t ncol;
        char cnt_char;
        bool break_before;
        int tab_size;
    };

    explicit NetlistBuilder(const char *fname);

    virtual void init(const std::vector<std::string> &inc_list, bool shell) = 0;

    void build();

    void add_cellview(const std::string &name, sch::cellview *cv,
                      const sch::netlist_map_t &cell_map, bool shell);

  protected:
    std::ofstream out_file;

    void write_instance(const std::string &name, const sch::instance &inst,
                        const sch::netlist_map_t &cell_map);

  private:
    virtual void write_end() = 0;

    virtual void write_cv_header(const std::string &name, const sch::term_t &in_terms,
                                 const sch::term_t &out_terms, const sch::term_t &io_terms) = 0;

    virtual void write_cv_end(const std::string &name) = 0;

    virtual void write_instance_helper(const std::string &name, const sch::instance &inst,
                                       const sch::cellview_info &info) = 0;
};

// forward declaration
struct time_struct;
struct binary_t;

class write_param_visitor {
  public:
    write_param_visitor(NetlistBuilder::LineBuilder *ptr, const std::string *key);

    void operator()(const std::string &v) const;
    void operator()(const int32_t &v) const;
    void operator()(const double &v) const;
    void operator()(const bool &v) const;
    void operator()(const time_struct &v) const;
    void operator()(const binary_t &v) const;

  private:
    NetlistBuilder::LineBuilder *ptr;
    const std::string *key;
};

} // namespace cbag

#endif // CBAG_NETLIST_NETLIST_H
