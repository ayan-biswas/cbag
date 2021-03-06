
#include <cbag/logging/logging.h>

#include <cbag/yaml/common.h>
#include <cbag/yaml/datatypes.h>

namespace YAML {
Node convert<cbag::value_t>::encode(const cbag::value_t &rhs) {
    Node root;
    root.push_back(rhs.index());
    std::visit(cbagyaml::to_yaml_visitor(&root), rhs);
    return root;
}

bool convert<cbag::value_t>::decode(const Node &node, cbag::value_t &rhs) {
    auto logger = cbag::get_cbag_logger();
    if (!node.IsSequence() || node.size() != 2) {
        logger->warn("cbag::value_t YAML decode: not a sequence or size != 2.  Node:\n{}",
                     cbagyaml::node_to_str(node));
        return false;
    }
    try {
        int value = node[0].as<int>();
        switch (value) {
        case 0:
            rhs = node[1].as<int_fast32_t>();
            return true;
        case 1:
            rhs = node[1].as<double_t>();
            return true;
        case 2:
            rhs = node[1].as<bool>();
            return true;
        case 3:
            rhs = node[1].as<std::string>();
            return true;
        case 4:
            rhs = node[1].as<cbag::time_struct>();
            return true;
        case 5:
            rhs = node[1].as<cbag::binary_t>();
            return true;
        default:
            logger->warn("cbag::value_t YAML decode: unexpected which value: {}.  Node:\n{}", value,
                         cbagyaml::node_to_str(node));
            return false;
        }
    } catch (...) {
        logger->warn("cbag::value_t YAML decode exception.  Node:\n{}",
                     cbagyaml::node_to_str(node));
        return false;
    }
}
} // namespace YAML
