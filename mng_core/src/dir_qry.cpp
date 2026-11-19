#include "mng_core/dir_qry.hpp"
#include "comp_mode.h"
#include "ent_err.hpp"
#include "ent_sys_qry.hpp"

#include <cerrno>
#include <dirent.h>
#include <memory>
#include <string_view>

static std::string_view get_ent_type(const std::filesystem::path &ent_path) {
  const struct stat st = get_ent_sys(ent_path, false);

  switch (st.st_mode & S_IFMT) {
  case S_IFDIR:
    return DIR_ICON;

  case S_IFREG:
    return FILE_ICON;

  case S_IFLNK: {
    const struct stat flw = get_ent_sys(ent_path, true);
    return S_ISDIR(flw.st_mode) ? SYMLINK_TO_DIR_ICON : SYMLINK_TO_FILE_ICON;
  }

  default:
    return FILE_ICON;
  }
}

static bool ign(std::string_view nm) {
  return nm == "." || nm == ".." || nm == "lost+found";
}

std::vector<mng_core::Ent> qry_dir(const std::filesystem::path &dir_path) {
  std::unique_ptr<DIR, int (*)(DIR *)> dir(opendir(dir_path.c_str()),
                                           &closedir);
  if (!dir)
    ent_err("opendir", dir_path);

  std::vector<mng_core::Ent> dir_buf;
  dir_buf.reserve(16);

  for (;;) {
    errno = 0;
    const dirent *ent = readdir(dir.get());
    if (!ent) {
      if (errno != 0)
        ent_err("readdir", dir_path);
      break;
    }
    if (ign(ent->d_name))
      continue;

    const std::filesystem::path ent_path = dir_path / ent->d_name;
    dir_buf.push_back(
        mng_core::Ent{std::string(get_ent_type(ent_path)), ent->d_name});
  }

  if (closedir(dir.release()) != 0)
    ent_err("closedir", dir_path);

  return dir_buf;
}
