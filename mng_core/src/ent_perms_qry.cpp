#include "mng_core/ent_perms_qry.hpp"
#include "comp_mode.h"
#include "ent_err.hpp"
#include "ent_sys_qry.hpp"

#include <cerrno>
#include <grp.h>
#include <pwd.h>
#include <string>
#include <system_error>

static std::string fmt_perms(mode_t mode, mode_t r, mode_t w, mode_t x) {
  std::string s;
  s += (mode & r) ? CAN_R_ICON : CAN_NOT_R_ICON;
  s += (mode & w) ? CAN_W_ICON : CAN_NOT_W_ICON;
  s += (mode & x) ? CAN_EXEC_ICON : CAN_NOT_EXEC_ICON;
  return s;
}

std::string mng_core::get_ent_own_perms(const std::filesystem::path &ent_path) {
  return fmt_perms(get_ent_sys(ent_path, false).st_mode, S_IRUSR, S_IWUSR,
                   S_IXUSR);
}

std::string mng_core::get_ent_grp_perms(const std::filesystem::path &ent_path) {
  return fmt_perms(get_ent_sys(ent_path, false).st_mode, S_IRGRP, S_IWGRP,
                   S_IXGRP);
}

std::string mng_core::get_ent_oth_perms(const std::filesystem::path &ent_path) {
  return fmt_perms(get_ent_sys(ent_path, false).st_mode, S_IROTH, S_IWOTH,
                   S_IXOTH);
}

std::string mng_core::get_ent_own(const std::filesystem::path &ent_path) {
  const struct stat st = get_ent_sys(ent_path, false);
  errno = 0;
  const struct passwd *pw = getpwuid(st.st_uid);
  if (!pw) {
    if (errno != 0)
      ent_err("getpwuid", ent_path);
    throw std::system_error(std::make_error_code(std::errc::no_such_process),
                            "no passwd entry for uid " +
                                std::to_string(st.st_uid));
  }
  return pw->pw_name;
}

std::string mng_core::get_ent_grp(const std::filesystem::path &ent_path) {
  const struct stat st = get_ent_sys(ent_path, false);
  errno = 0;
  const struct group *gr = getgrgid(st.st_gid);
  if (!gr) {
    if (errno != 0)
      ent_err("getgrgid", ent_path);
    throw std::system_error(std::make_error_code(std::errc::no_such_process),
                            "no group entry for gid " +
                                std::to_string(st.st_gid));
  }
  return gr->gr_name;
}
