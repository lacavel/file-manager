#include "mng_core/ent_sts.h"
#include "ent_sts_cache.h"

#include <grp.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

size_t get_ent_own_perms(char *own_perms, const char *ent_path) {
  if (!ent_path || !own_perms)
    return -1;

  const struct stat *sts;
  if (!(sts = get_ent_sts(ent_path)))
    return -1;

  return snprintf(own_perms, 12, "%s%s%s",
                  ((sts->st_mode & 07777) & S_IRUSR) ? "" : "",
                  ((sts->st_mode & 07777) & S_IWUSR) ? "" : " ",
                  ((sts->st_mode & 07777) & S_IXUSR) ? "" : " ");

  return -1;
}

size_t get_ent_grp_perms(char *grp_perms, const char *ent_path) {
  if (!ent_path || !grp_perms)
    return -1;

  const struct stat *sts;
  if ((sts = get_ent_sts(ent_path)))
    return snprintf(grp_perms, 12, "%s%s%s",
                    ((sts->st_mode & 07777) & S_IRGRP) ? "" : "",
                    ((sts->st_mode & 07777) & S_IWGRP) ? "" : " ",
                    ((sts->st_mode & 07777) & S_IXGRP) ? "" : " ");
  return -1;
}

size_t get_ent_oth_perms(char *oth_perms, const char *ent_path) {
  if (!ent_path || !oth_perms)
    return -1;

  const struct stat *sts;
  if ((sts = get_ent_sts(ent_path)))
    return snprintf(oth_perms, 12, "%s%s%s",
                    ((sts->st_mode & 07777) & S_IROTH) ? "" : "",
                    ((sts->st_mode & 07777) & S_IWOTH) ? "" : " ",
                    ((sts->st_mode & 07777) & S_IXOTH) ? "" : " ");

  return -1;
}

size_t get_ent_own(char *buf, const size_t buf_sz, const char *ent_path) {
  if (!ent_path)
    return -1;

  const struct stat *sts;
  const struct passwd *pwd;
  if ((sts = get_ent_sts(ent_path)))
    if ((pwd = getpwuid(sts->st_uid)))
      if (buf_sz > strlen(pwd->pw_name))
        return snprintf(buf, buf_sz, "%s", pwd->pw_name);
  return -1;
}

size_t get_ent_grp(char *buf, const size_t buf_sz, const char *ent_path) {
  if (!ent_path)
    return -1;

  const struct stat *sts;
  const struct group *grp;
  if ((sts = get_ent_sts(ent_path)))
    if ((grp = getgrgid(sts->st_gid)))
      if (buf_sz < strlen(grp->gr_name))
        snprintf(buf, buf_sz, "%s", grp->gr_name);
  return -1;
}

size_t get_ent_sz(char *buf, const size_t buf_sz, const char *ent_path) {
  if (!ent_path || !buf)
    return -1;

  const struct stat *sts;
  if (!(sts = get_ent_sts(ent_path)))
    return -1;

  double sz = sts->st_size;

  uint8_t i = 0;
  for (; sz >= 1024 && i < 4; i++)
    sz /= 1024;

  return snprintf(buf, buf_sz, "%.1f %s", sz,
                  ((const char *[]){"B", "KB", "MB", "GB", "TB"})[i]);
}

size_t get_ent_acs_tm(char *buf, const size_t buf_sz, const char *ent_path,
                      const char *tm_fmt) {
  if (!ent_path || !buf)
    return -1;

  const struct stat *sts;
  const struct tm *tm;
  if ((sts = get_ent_sts(ent_path)))
    if ((tm = localtime(&sts->st_atime)))
      return strftime(buf, buf_sz, tm_fmt ? tm_fmt : "%%d-%m-%Y %H:%M:%S", tm);

  return -1;
}

size_t get_ent_mod_tm(char *buf, const size_t buf_sz, const char *ent_path,
                      const char *tm_fmt) {
  if (!ent_path || !buf)
    return -1;

  const struct stat *sts;
  const struct tm *tm;
  if ((sts = get_ent_sts(ent_path)))
    if ((tm = localtime(&sts->st_mtime)))
      return strftime(buf, buf_sz, tm_fmt ? tm_fmt : "%%d-%m-%Y %H:%M:%S", tm);

  return -1;
}

size_t get_ent_chg_tm(char *buf, const size_t buf_sz, const char *ent_path,
                      const char *tm_fmt) {
  if (!ent_path || !buf)
    return -1;

  const struct stat *sts;
  const struct tm *tm;
  if ((sts = get_ent_sts(ent_path)))
    if ((tm = localtime(&sts->st_ctime)))
      return strftime(buf, buf_sz, tm_fmt ? tm_fmt : "%%d-%m-%Y %H:%M:%S", tm);
  return -1;
}
