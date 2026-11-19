#include "mng_core/ent_ts_qry.hpp"
#include "ent_sys_qry.hpp"

#include <cerrno>
#include <stdexcept>
#include <string>
#include <system_error>
#include <time.h>

static std::string fmt_timestamp(std::time_t ts, std::string_view fmt) {
  struct tm lt{};
  if (localtime_r(&ts, &lt) == nullptr)
    throw std::system_error(errno, std::generic_category(), "localtime_r");

  const std::string f = std::string(fmt) + '\1';

  std::string buf(64, '\0');
  while (buf.size() <= 4096) {
    const std::size_t n = strftime(buf.data(), buf.size(), f.c_str(), &lt);
    if (n != 0) {
      buf.resize(n - 1);
      return buf;
    }
    buf.resize(buf.size() * 2);
  }
  throw std::length_error("timestamp format exceeded 4096 bytes");
}

std::string get_ent_acs_ts(const std::filesystem::path &ent_path,
                           std::string_view tm_fmt) {
  return fmt_timestamp(get_ent_sys(ent_path, false).st_atime, tm_fmt);
}

std::string get_ent_mod_ts(const std::filesystem::path &ent_path,
                           std::string_view tm_fmt) {
  return fmt_timestamp(get_ent_sys(ent_path, false).st_mtime, tm_fmt);
}

std::string get_ent_chg_ts(const std::filesystem::path &ent_path,
                           std::string_view tm_fmt) {
  return fmt_timestamp(get_ent_sys(ent_path, false).st_ctime, tm_fmt);
}
