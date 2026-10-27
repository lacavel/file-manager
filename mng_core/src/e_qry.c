#include "mng_core/e_qry.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

ssize_t qry_dir(char ***dir_buf, const char *dir_path, size_t *max_ent_nm_len) {
  if (!dir_buf || !dir_path || !max_ent_nm_len)
    return -1;

  *max_ent_nm_len = 0;
  size_t ent_cnt = 0, cpty = 16;

  DIR *dir = opendir(dir_path);
  if (!dir)
    return -1;

  char **buf = malloc(cpty * sizeof(*buf));
  if (!buf)
    goto err;

  struct dirent *e;

  for (;;) {
    errno = 0;
    e = readdir(dir);

    if (!e) {
      if (errno != 0)
        goto err;
      break;
    }
    if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0 ||
        strcmp(e->d_name, "lost+found") == 0)
      continue;

    if (ent_cnt == cpty) {
      if (cpty > SIZE_MAX / 2)
        goto err;

      cpty *= 2;
      if (cpty > SIZE_MAX / sizeof(*buf))
        goto err;

      char **buf_tmp = realloc(buf, cpty * sizeof(*buf));
      if (!buf_tmp)
        goto err;
      buf = buf_tmp;
    }

    const size_t ent_nm_len = strlen(e->d_name);

    buf[ent_cnt] = malloc(ent_nm_len + 1);
    if (!buf[ent_cnt])
      goto err;
    memcpy(buf[ent_cnt], e->d_name, ent_nm_len + 1);

    if (ent_nm_len > *max_ent_nm_len)
      *max_ent_nm_len = ent_nm_len;

    ent_cnt++;
  }

  {
    const int cl = closedir(dir);
    dir = NULL;

    if (cl == -1)
      goto err;
  }

  *dir_buf = buf;
  return ent_cnt;

err:
  if (dir)
    closedir(dir);

  if (buf)
    free_dir(buf, ent_cnt);
  return -1;
}

void free_dir(char **dir_buf, size_t ent_cnt) {
  if (!dir_buf)
    return;

  for (size_t i = 0; i < ent_cnt; i++)
    free(dir_buf[i]);
  free(dir_buf);
}
