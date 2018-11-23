
#include <cbag/logging/logging.h>

#include <cbag/common/typedefs.h>

#include <cbag/yaml/common.h>
#include <cbag/yaml/enum.h>
#include <cbag/yaml/transformation.h>

namespace YAML {

Node convert<cbag::transformation>::encode(const cbag::transformation &rhs) {
    Node root;
    root.push_back(rhs.x());
    root.push_back(rhs.y());
    root.push_back(rhs.orient_code());
    return root;
}

bool convert<cbag::transformation>::decode(const Node &node, cbag::transformation &rhs) {
    auto logger = cbag::get_cbag_logger();
    if (!node.IsSequence() || node.size() != 3) {
        logger->warn("cbag::transformation YAML decode: not a sequence or size != 3.  Node:\n{}",
                     cbagyaml::node_to_str(node));
        return false;
    }
    try {
        rhs.set_location(node[0].as<cbag::coord_t>(), node[1].as<cbag::coord_t>());
        rhs.set_orient_code(node[2].as<uint32_t>());
    } catch (...) {
        logger->warn("cbag::transformation YAML decode exception.  Node:\n{}",
                     cbagyaml::node_to_str(node));
        return false;
    }
    return true;
}

} // namespace YAML