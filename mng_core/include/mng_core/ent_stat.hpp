#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>

namespace mng_core {

using EntType = std::filesystem::file_type;
using EntPerms = std::filesystem::perms;

using EntId = std::uintmax_t;

using EntTs = std::chrono::sys_time<std::chrono::nanoseconds>;

struct EntStat {
  EntType type = EntType::unknown;
  EntPerms perms = EntPerms::unknown;

  std::optional<std::uintmax_t> sz;
  std::optional<std::uintmax_t> blk_cnt;
  std::optional<std::uintmax_t> blk_sz;
  std::optional<std::uintmax_t> lnk_cnt;

  std::optional<EntId> uid;
  std::optional<EntId> gid;

  std::optional<EntId> ino;
  std::optional<EntId> dev;
  std::optional<EntId> rdev;

  std::optional<EntTs> acs_ts;
  std::optional<EntTs> mod_ts;
  std::optional<EntTs> chg_ts;
  std::optional<EntTs> birth_ts;
};

} // namespace mng_core
