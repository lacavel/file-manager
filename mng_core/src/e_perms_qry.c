#include "mng_core/e_perms_qry.h"
#include "e_attr.h"

#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>

int get_ent_own_perms(char *own_perms_buf, const char *ent_path) {
  if (!ent_path || !own_perms_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  mode_t mode = sts->st_mode & 07777;
  char *p = own_perms_buf;

  p = stpcpy(p, (mode & S_IRUSR) ? "" : "");
  p = stpcpy(p, (mode & S_IWUSR) ? "" : " ");
  p = stpcpy(p, (mode & S_IXUSR) ? "" : " ");

  return p - own_perms_buf;
}

int get_ent_grp_perms(char *grp_perms_buf, const char *ent_path) {
  if (!ent_path || !grp_perms_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  mode_t mode = sts->st_mode & 07777;
  char *p = grp_perms_buf;

  p = stpcpy(p, (mode & S_IRGRP) ? "" : "");
  p = stpcpy(p, (mode & S_IWGRP) ? "" : " ");
  p = stpcpy(p, (mode & S_IXGRP) ? "" : " ");

  return p - grp_perms_buf;
}

int get_ent_oth_perms(char *oth_perms_buf, const char *ent_path) {
  if (!ent_path || !oth_perms_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  mode_t mode = sts->st_mode & 07777;
  char *p = oth_perms_buf;

  p = stpcpy(p, (mode & S_IROTH) ? "" : "");
  p = stpcpy(p, (mode & S_IWOTH) ? "" : " ");
  p = stpcpy(p, (mode & S_IXOTH) ? "" : " ");

  return p - oth_perms_buf;
}

int get_ent_own(char *own_buf, const char *ent_path) {
  if (!ent_path || !own_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  const struct passwd *pwd = getpwuid(sts->st_uid);
  if (!pwd)
    return -1;

  size_t len = strlen(pwd->pw_name);
  memcpy(own_buf, pwd->pw_name, len + 1);
  return (int)len;
}

int get_ent_grp(char *grp_buf, const char *ent_path) {
  if (!ent_path || !grp_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  const struct group *grp = getgrgid(sts->st_gid);
  if (!grp)
    return -1;

  size_t len = strlen(grp->gr_name);
  memcpy(grp_buf, grp->gr_name, len + 1);
  return (int)len;
}
