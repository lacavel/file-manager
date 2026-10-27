#include "e_attr.h"

#include <limits.h>
#include <string.h>

static struct stat ent_sts_last;
static char ent_path_last[PATH_MAX];

const struct stat *get_ent_sts(const char *ent_path, const bool flw) {
  if (!ent_path)
    return NULL;
  if (strcmp(ent_path_last, ent_path) == 0)
    return &ent_sts_last;
  else if ((flw ? stat : lstat)(ent_path, &ent_sts_last) == -1)
    return NULL;

  strncpy(ent_path_last, ent_path, PATH_MAX - 1);
  ent_path_last[PATH_MAX - 1] = '\0';
  return &ent_sts_last;
}
