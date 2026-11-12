#include "e_sts_qry.h"
#include <limits.h>
#include <stdbool.h>
#include <string.h>

static _Thread_local struct stat ent_sts_last;
static _Thread_local char ent_path_last[PATH_MAX];
static _Thread_local bool ent_last_flw;
static _Thread_local bool ent_cache_valid = false;

const struct stat *get_ent_sts(const char *ent_path, const bool flw) {
  if (!ent_path)
    return NULL;

  if (ent_cache_valid && ent_last_flw == flw &&
      strcmp(ent_path_last, ent_path) == 0)
    return &ent_sts_last;

  if ((flw ? stat : lstat)(ent_path, &ent_sts_last) == -1) {
    ent_cache_valid = false;
    return NULL;
  }

  size_t path_len = strlen(ent_path);

  if (path_len >= PATH_MAX)
    path_len = PATH_MAX - 1;

  memcpy(ent_path_last, ent_path, path_len);

  ent_path_last[path_len] = '\0';

  ent_last_flw = flw;
  ent_cache_valid = true;

  return &ent_sts_last;
}
