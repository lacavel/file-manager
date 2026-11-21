#include "mng_core/ent_sz_qry.hpp"
#include "ent_sys_qry.hpp"

#include <array>
#include <format>

std::string mng_core::get_ent_sz(const std::filesystem::path &ent_path,
                                 bool fmt) {
  const struct stat st = get_ent_sys(ent_path, true);

  if (!fmt || st.st_size < 1024)
    return std::format("{} B", st.st_size);

  static constexpr std::array units{"B", "KB", "MB", "GB", "TB", "PB"};
  double v = static_cast<double>(st.st_size);
  std::size_t i = 0;
  while (v >= 1024.0 && i + 1 < units.size()) {
    v /= 1024.0;
    ++i;
  }
  return std::format("{:.2f} {}", v, units[i]);
}
