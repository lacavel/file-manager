#include "ent_fmt.hpp"
#include "comp_mode.h"

#include <array>
#include <ctime>
#include <format>

namespace mng {

using namespace mng_core;
std::string fmt_perm_icons(EntPerms perms, EntPerms r, EntPerms w, EntPerms x) {
  using P = EntPerms;

  if (perms == P::unknown)
    return "? ? ?";

  const auto has = [perms](P p) { return (perms & p) != P::none; };

  std::string ret;

  ret += has(r) ? CAN_R_ICON : CAN_NOT_R_ICON;
  ret += has(w) ? CAN_W_ICON : CAN_NOT_W_ICON;
  ret += has(x) ? CAN_EXEC_ICON : CAN_NOT_EXEC_ICON;

  return ret;
}

std::string fmt_sz(std::uintmax_t sz) {
  constexpr std::array units{"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};

  double val = static_cast<double>(sz);
  std::size_t unit = 0;

  while (val >= 1024.0 && unit + 1 < units.size()) {
    val /= 1024.0;
    ++unit;
  }

  if (unit == 0)
    return std::format("{} B", sz);

  return std::format("{:.2f} {}", val, units[unit]);
}

std::string fmt_ts(EntTs ts) {
  const auto tp = std::chrono::floor<std::chrono::seconds>(ts);

  const auto tm_val = std::chrono::system_clock::to_time_t(tp);
  std::tm tm{};
  if (!localtime_r(&tm_val, &tm))
    return "?";
  char buf[64]{};
  if (!std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm))
    return "?";

  return buf;
}

std::string fmt_perms(EntPerms perms) {
  using P = EntPerms;

  if (perms == P::unknown)
    return "?????????";

  const auto has = [perms](P p) { return (perms & p) != P::none; };

  std::string ret = "---------";

  if (has(P::owner_read))
    ret[0] = 'r';
  if (has(P::owner_write))
    ret[1] = 'w';
  if (has(P::owner_exec))
    ret[2] = 'x';

  if (has(P::group_read))
    ret[3] = 'r';
  if (has(P::group_write))
    ret[4] = 'w';
  if (has(P::group_exec))
    ret[5] = 'x';

  if (has(P::others_read))
    ret[6] = 'r';
  if (has(P::others_write))
    ret[7] = 'w';
  if (has(P::others_exec))
    ret[8] = 'x';

  if (has(P::set_uid))
    ret[2] = has(P::owner_exec) ? 's' : 'S';

  if (has(P::set_gid))
    ret[5] = has(P::group_exec) ? 's' : 'S';

  if (has(P::sticky_bit))
    ret[8] = has(P::others_exec) ? 't' : 'T';

  return ret;
}

std::string_view fmt_type(EntType type) {
  switch (type) {
  case EntType::regular:
    return "Regular file";

  case EntType::directory:
    return "Directory";

  case EntType::symlink:
    return "Symbolic link";

  case EntType::block:
    return "Block device";

  case EntType::character:
    return "Character device";

  case EntType::fifo:
    return "FIFO";

  case EntType::socket:
    return "Socket";

  default:
    return "Unknown";
  }
}

std::string_view get_ent_icon(EntType type) {
  switch (type) {
  case EntType::directory:
    return DIR_ICON;

  case EntType::symlink:
    return SYMLINK_TO_FILE_ICON;

  case EntType::regular:
    return FILE_ICON;

  default:
    return "? ";
  }
}

} // namespace mng
