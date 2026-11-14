#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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

int nav_up(char *path_buf, const char *path) {
  if (!path_buf || !path)
    return -1;

  int path_len = strlen(path);
  if (path_len == 0)
    return -1;

  while (path_len > 0 && path[path_len - 1] == '/')
    path_len--;

  if (path_len == 0) {
    path_buf[0] = '/';
    path_buf[1] = '\0';
    return 1;
  }

  int last_slash = -1;
  for (int i = path_len - 1; i >= 0; i--) {
    if (path[i] == '/') {
      last_slash = i;
      break;
    }
  }

  if (last_slash == 0) {
    path_buf[0] = '/';
    path_buf[1] = '\0';
    return 1;
  } else if (last_slash > 0) {
    memcpy(path_buf, path, last_slash);
    path_buf[last_slash] = '\0';
    return last_slash;
  } else {
    path_buf[0] = '.';
    path_buf[1] = '\0';
    return 1;
  }
}

int nav_down(char *path_buf, const char *path, const char *ent_nm) {
  if (!path_buf || !path || !ent_nm)
    return -1;

  unsigned int path_len = strlen(path);
  unsigned int ent_nm_len = strlen(ent_nm);

  int needs_slash = (path_len > 0 && path[path_len - 1] != '/') ? 1 : 0;

  memcpy(path_buf, path, path_len);

  if (needs_slash) {
    path_buf[path_len] = '/';
    memcpy(path_buf + path_len + 1, ent_nm, ent_nm_len);
    path_buf[path_len + 1 + ent_nm_len] = '\0';
    return path_len + 1 + ent_nm_len;
  } else {
    memcpy(path_buf + path_len, ent_nm, ent_nm_len);
    path_buf[path_len + ent_nm_len] = '\0';
    return path_len + ent_nm_len;
  }
}
