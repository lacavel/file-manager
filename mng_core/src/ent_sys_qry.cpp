#include "ent_sys_qry.hpp"
#include "ent_err.hpp"

struct stat get_ent_sys(const std::filesystem::path &ent_path, bool flw) {
  struct stat st{};
  if ((flw ? stat : lstat)(ent_path.c_str(), &st) != 0)
    mng_core::ent_err(flw ? "stat" : "lstat", ent_path);
  return st;
}
