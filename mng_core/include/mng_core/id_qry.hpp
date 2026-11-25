#pragma once

#include "mng_core/ent_stat.hpp"

#include <optional>
#include <string>

namespace mng_core {

std::optional<std::string> qry_usr(EntId uid);
std::optional<std::string> qry_grp(EntId gid);

} // namespace mng_core
