#include "ent_sts_cache.h"

#include <limits.h>
#include <string.h>

static struct stat ent_sts_cache;
static char ent_path_cache[PATH_MAX];

const struct stat *get_ent_sts(const char *ent_path) {
  if (!ent_path)
    return NULL;
  if (strcmp(ent_path_cache, ent_path) == 0)
    return &ent_sts_cache;
  else if (lstat(ent_path, &ent_sts_cache) == -1)
    return NULL;

  strncpy(ent_path_cache, ent_path, PATH_MAX - 1);
  ent_path_cache[PATH_MAX - 1] = '\0';
  return &ent_sts_cache;
}
