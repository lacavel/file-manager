#pragma once

#include "mng_core/ent_stat.hpp"

#include <filesystem>
#include <system_error>

namespace mng_core {

enum class LnkMode { NoFollow, Follow };

EntStat qry_ent(const std::filesystem::path &ent_path,
                LnkMode mode = LnkMode::NoFollow);

EntStat qry_ent(const std::filesystem::path &ent_path, std::error_code &err,
                LnkMode mode = LnkMode::NoFollow);

} // namespace mng_core
