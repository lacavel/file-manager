#include "mng_core/e_type_qry.h"
#include "e_sts_qry.h"
#include <string.h>
#include <sys/stat.h>

#define FILE_ICON " "
#define DIR_ICON " "
#define SYMLINK_TO_FILE_ICON " "
#define SYMLINK_TO_DIR_ICON " "

char *get_ent_type(const char *ent_path) {
  if (!ent_path)
    return NULL;

  const struct stat *sts = get_ent_sts(ent_path, false);
  if (!sts)
    return NULL;

  switch (sts->st_mode & S_IFMT) {
  case S_IFDIR:
    return DIR_ICON;

  case S_IFREG:
    return FILE_ICON;

  case S_IFLNK: {
    const struct stat *flw = get_ent_sts(ent_path, true);
    if (!flw)
      return NULL;

    if (S_ISDIR(flw->st_mode))
      return SYMLINK_TO_DIR_ICON;
    else
      return SYMLINK_TO_FILE_ICON;
  }

  default:
    return FILE_ICON;
  }
}
