#include "mng_core/e_tm_qry.h"
#include "e_sts_qry.h"

#include <sys/stat.h>
#include <time.h>

int get_ent_acs_tm(char *acs_tm_buf, const char *ent_path, const char *tm_fmt) {
  if (!ent_path || !acs_tm_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  const struct tm *tm = localtime(&sts->st_atime);
  if (!tm)
    return -1;

  return (int)strftime(acs_tm_buf, 32, tm_fmt ? tm_fmt : "%d-%m-%Y %H:%M:%S",
                       tm);
}

int get_ent_mod_tm(char *mod_tm_buf, const char *ent_path, const char *tm_fmt) {
  if (!ent_path || !mod_tm_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  const struct tm *tm = localtime(&sts->st_mtime);
  if (!tm)
    return -1;

  return (int)strftime(mod_tm_buf, 32, tm_fmt ? tm_fmt : "%d-%m-%Y %H:%M:%S",
                       tm);
}

int get_ent_chg_tm(char *chg_tm_buf, const char *ent_path, const char *tm_fmt) {
  if (!ent_path || !chg_tm_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  const struct tm *tm = localtime(&sts->st_ctime);
  if (!tm)
    return -1;

  return (int)strftime(chg_tm_buf, 32, tm_fmt ? tm_fmt : "%d-%m-%Y %H:%M:%S",
                       tm);
}
