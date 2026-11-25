#include "mng_core/ent_qry.hpp"
#include "ent_sys_qry.hpp"

#include <fcntl.h>

namespace mng_core {

EntStat qry_ent(const std::filesystem::path &ent_path, std::error_code &err,
                LnkMode mode) {
  return qry_ent_at(AT_FDCWD, ent_path.c_str(), err, mode);
}

EntStat qry_ent(const std::filesystem::path &ent_path, LnkMode mode) {
  std::error_code err;

  auto st = qry_ent(ent_path, err, mode);

  if (err)
    throw std::filesystem::filesystem_error("qry_ent", ent_path, err);

  return st;
}

} // namespace mng_core
