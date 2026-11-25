#include "ent_sys_qry.hpp"

#include <cerrno>
#include <fcntl.h>

namespace mng_core {

static EntType conv_type(mode_t mode) noexcept {
  switch (mode & S_IFMT) {
  case S_IFREG:
    return EntType::regular;

  case S_IFDIR:
    return EntType::directory;

  case S_IFLNK:
    return EntType::symlink;

  case S_IFBLK:
    return EntType::block;

  case S_IFCHR:
    return EntType::character;

  case S_IFIFO:
    return EntType::fifo;

  case S_IFSOCK:
    return EntType::socket;

  default:
    return EntType::unknown;
  }
}

static EntPerms conv_perms(mode_t mode) noexcept {
  return static_cast<EntPerms>(mode & 07777);
}

static EntTs conv_ts(const struct timespec &ts) {
  using namespace std::chrono;

  return EntTs{seconds{ts.tv_sec} + nanoseconds{ts.tv_nsec}};
}

EntStat conv_ent_sys(const struct stat &st) {
  EntStat ret;

  ret.type = conv_type(st.st_mode);
  ret.perms = conv_perms(st.st_mode);

  if (st.st_size >= 0)
    ret.sz = static_cast<std::uintmax_t>(st.st_size);

  if (st.st_blocks >= 0)
    ret.blk_cnt = static_cast<std::uintmax_t>(st.st_blocks);

  if (st.st_blksize >= 0)
    ret.blk_sz = static_cast<std::uintmax_t>(st.st_blksize);

  ret.lnk_cnt = static_cast<std::uintmax_t>(st.st_nlink);

  ret.uid = static_cast<EntId>(st.st_uid);
  ret.gid = static_cast<EntId>(st.st_gid);

  ret.ino = static_cast<EntId>(st.st_ino);
  ret.dev = static_cast<EntId>(st.st_dev);
  ret.rdev = static_cast<EntId>(st.st_rdev);

  ret.acs_ts = conv_ts(st.st_atim);
  ret.mod_ts = conv_ts(st.st_mtim);
  ret.chg_ts = conv_ts(st.st_ctim);

  return ret;
}

EntStat qry_ent_at(int fd, const char *nm, std::error_code &err, LnkMode mode) {
  struct stat st{};

  const int flags = mode == LnkMode::NoFollow ? AT_SYMLINK_NOFOLLOW : 0;

  if (fstatat(fd, nm, &st, flags) != 0) {
    err.assign(errno, std::generic_category());
    return {};
  }

  err.clear();

  return conv_ent_sys(st);
}

} // namespace mng_core
