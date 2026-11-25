#pragma once

#include "mng_core/ent_qry.hpp"

#include <sys/stat.h>

namespace mng_core {

EntStat conv_ent_sys(const struct stat &st);

EntStat qry_ent_at(int fd, const char *nm, std::error_code &err,
                   LnkMode mode = LnkMode::NoFollow);

} // namespace mng_core
