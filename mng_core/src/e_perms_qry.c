#include "mng_core/e_perms_qry.h"
#include "comp_mode.h"
#include "e_sts_qry.h"

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

  p = stpcpy(p, (mode & S_IRUSR) ? CAN_R_ICON : CAN_NOT_R_ICON);
  p = stpcpy(p, (mode & S_IWUSR) ? CAN_W_ICON : CAN_NOT_W_ICON);
  p = stpcpy(p, (mode & S_IXUSR) ? CAN_EXEC_ICON : CAN_NOT_EXEC_ICON);

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

  p = stpcpy(p, (mode & S_IRGRP) ? CAN_R_ICON : CAN_NOT_R_ICON);
  p = stpcpy(p, (mode & S_IWGRP) ? CAN_W_ICON : CAN_NOT_W_ICON);
  p = stpcpy(p, (mode & S_IXGRP) ? CAN_EXEC_ICON : CAN_NOT_EXEC_ICON);

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

  p = stpcpy(p, (mode & S_IROTH) ? CAN_R_ICON : CAN_NOT_R_ICON);
  p = stpcpy(p, (mode & S_IWOTH) ? CAN_W_ICON : CAN_NOT_W_ICON);
  p = stpcpy(p, (mode & S_IXOTH) ? CAN_EXEC_ICON : CAN_NOT_EXEC_ICON);

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
