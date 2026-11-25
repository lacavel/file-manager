#include "mng_core/id_qry.hpp"

#include <cerrno>
#include <grp.h>
#include <limits>
#include <pwd.h>
#include <system_error>
#include <vector>

namespace mng_core {

std::optional<std::string> qry_usr(EntId uid) {
  if (uid > std::numeric_limits<uid_t>::max())
    return std::nullopt;

  std::vector<char> buf(1024);

  for (;;) {
    struct passwd pw{};
    struct passwd *res = nullptr;

    const int err =
        getpwuid_r(static_cast<uid_t>(uid), &pw, buf.data(), buf.size(), &res);

    if (err == ERANGE) {
      buf.resize(buf.size() * 2);
      continue;
    }

    if (err)
      throw std::system_error(err, std::generic_category(), "getpwuid_r");

    if (!res)
      return std::nullopt;

    return std::string(res->pw_name);
  }
}

std::optional<std::string> qry_grp(EntId gid) {
  if (gid > std::numeric_limits<gid_t>::max())
    return std::nullopt;

  std::vector<char> buf(1024);

  for (;;) {
    struct group gr{};
    struct group *res = nullptr;

    const int err =
        getgrgid_r(static_cast<gid_t>(gid), &gr, buf.data(), buf.size(), &res);

    if (err == ERANGE) {
      buf.resize(buf.size() * 2);
      continue;
    }

    if (err)
      throw std::system_error(err, std::generic_category(), "getgrgid_r");

    if (!res)
      return std::nullopt;

    return std::string(res->gr_name);
  }
}

} // namespace mng_core
