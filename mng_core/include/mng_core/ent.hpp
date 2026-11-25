#pragma once

#include "mng_core/ent_stat.hpp"

#include <filesystem>
#include <optional>
#include <system_error>

namespace mng_core {

struct Ent {
  std::filesystem::path nm;

  EntType type = EntType::unknown;

  std::optional<EntStat> st;

  std::error_code st_err;
};

} // namespace mng_core
