#ifndef CBAGOA_OA_UTIL_H
#define CBAGOA_OA_UTIL_H

#include <cstdint>
#include <string>

#include <oa/oaDesignDB.h>

namespace spdlog {
class logger;
}

namespace cbagoa {

// constants
constexpr uint32_t LIB_ACCESS_TIMEOUT = 1;

oa::oaTech *read_tech(const oa::oaNativeNS &ns, const std::string &lib_name);

oa::oaDesign *open_design(const oa::oaNativeNS &ns, spdlog::logger &logger,
                          const std::string &lib_name, const std::string &cell_name,
                          const std::string &view_name, char mode,
                          oa::oaReservedViewTypeEnum view_enum);

void handle_oa_exceptions(spdlog::logger &logger);

oa::oaLib *open_library_read(const oa::oaNativeNS &ns, const std::string &lib_name);

} // namespace cbagoa

#endif
