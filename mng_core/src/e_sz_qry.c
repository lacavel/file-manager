#include "mng_core/e_sz_qry.h"
#include "e_sts_qry.h"
#include <stdio.h>

int get_ent_sz(char *sz_buf, const char *ent_path, bool fmt) {
  if (!ent_path || !sz_buf)
    return -1;
  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return -1;

  const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
  double sz = (double)sts->st_size, sz_fmt = sz;

  unsigned int i = 0;
  for (; (sz_fmt >= 1024.0 && i < 5); i++)
    sz_fmt /= 1024.0;

  if (i == 0 || !fmt)
    return snprintf(sz_buf, 64, "%11d %s", (int)sz, units[0]);
  else
    return snprintf(sz_buf, 64, "%.2f %s", sz_fmt, units[i]);
}
