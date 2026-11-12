#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void get_abs_path(char *path_buf, const char *ent_path) {
  if (!path_buf)
    return;

  struct stat sts;
  if (ent_path && realpath(ent_path, path_buf) && stat(path_buf, &sts) == 0 &&
      S_ISDIR(sts.st_mode))
    return;

  if (!getcwd(path_buf, PATH_MAX)) {
    path_buf[0] = '/';
    path_buf[1] = '\0';
  }
}
