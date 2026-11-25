#include "mng_core/dir_qry.hpp"
#include "ent_sys_qry.hpp"

#include <cerrno>
#include <dirent.h>
#include <memory>
#include <string_view>
#include <utility>

namespace mng_core {

static bool ign(std::string_view nm, DirVis vis) {
  switch (vis) {
  case DirVis::Normal:
    return nm.starts_with('.');

  case DirVis::AlmostAll:
    return nm == "." || nm == "..";

  case DirVis::All:
    return false;
  }

  return false;
}

static EntType get_ent_type(const dirent &ent) {
#ifdef _DIRENT_HAVE_D_TYPE
  switch (ent.d_type) {
  case DT_REG:
    return EntType::regular;

  case DT_DIR:
    return EntType::directory;

  case DT_LNK:
    return EntType::symlink;

  case DT_BLK:
    return EntType::block;

  case DT_CHR:
    return EntType::character;

  case DT_FIFO:
    return EntType::fifo;

  case DT_SOCK:
    return EntType::socket;

  default:
    return EntType::unknown;
  }
#else
  (void)ent;
  return EntType::unknown;
#endif
}

std::vector<Ent> qry_dir(const std::filesystem::path &dir_path,
                         const DirQryOpts &opts) {
  std::unique_ptr<DIR, int (*)(DIR *)> dir(opendir(dir_path.c_str()),
                                           &closedir);

  if (!dir)
    throw std::filesystem::filesystem_error(
        "opendir", dir_path, std::error_code(errno, std::generic_category()));

  const int fd = dirfd(dir.get());

  if (fd < 0)
    throw std::filesystem::filesystem_error(
        "dirfd", dir_path, std::error_code(errno, std::generic_category()));

  std::vector<Ent> dir_buf;
  dir_buf.reserve(16);

  for (;;) {
    errno = 0;

    const dirent *ent = readdir(dir.get());

    if (!ent) {
      if (errno != 0)
        throw std::filesystem::filesystem_error(
            "readdir", dir_path,
            std::error_code(errno, std::generic_category()));

      break;
    }

    if (ign(ent->d_name, opts.vis))
      continue;

    Ent ret;

    ret.nm = ent->d_name;
    ret.type = get_ent_type(*ent);

    if (opts.mode == QryMode::Full || ret.type == EntType::unknown) {
      auto st = qry_ent_at(fd, ent->d_name, ret.st_err);

      if (!ret.st_err) {
        ret.type = st.type;

        if (opts.mode == QryMode::Full)
          ret.st = std::move(st);
      }
    }

    dir_buf.push_back(std::move(ret));
  }

  if (closedir(dir.release()) != 0)
    throw std::filesystem::filesystem_error(
        "closedir", dir_path, std::error_code(errno, std::generic_category()));

  return dir_buf;
}

} // namespace mng_core
