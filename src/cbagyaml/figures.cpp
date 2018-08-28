/** \file figures.cpp
 *  \brief This file defines YAML serialization methods for datatypes and
 * figures.
 *
 *  \author Eric Chang
 *  \date   2018/07/12
 */

#include <spdlog/spdlog.h>

#include <cbagyaml/datatypes.h>
#include <cbagyaml/figures.h>

namespace YAML {
Node convert<cbag::sch::pin_fig_t>::encode(const cbag::sch::pin_fig_t &rhs) {
    Node root;
    root.push_back(rhs.index());
    std::visit(cbag::to_yaml_visitor(&root), rhs);
    return root;
}

bool convert<cbag::sch::pin_fig_t>::decode(const Node &node, cbag::sch::pin_fig_t &rhs) {
    auto logger = spdlog::get("cbag");
    if (!node.IsSequence() || node.size() != 2) {
        logger->warn("cbag::sch::pin_fig_t YAML decode: not a sequence or size != 2.  Node:\n{}",
                     yaml::serialization::node_to_str(node));
        return false;
    }
    try {
        int value = node[0].as<int>();
        switch (value) {
        case 0:
            rhs = node[1].as<cbag::sch::rectangle>();
            return true;
        case 1:
            rhs = node[1].as<cbag::sch::pin_object>();
            return true;
        default:
            logger->warn("cbag::sch::pin_fig_t YAML decode: unexpected which value: {}.  Node:\n{}",
                         value, yaml::serialization::node_to_str(node));
            return false;
        }
    } catch (...) {
        logger->warn("cbag::sch::pin_fig_t YAML decode exception.  Node:\n{}",
                     yaml::serialization::node_to_str(node));
        return false;
    }
}

} // namespace YAML