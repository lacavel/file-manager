#include "mng_core/e_qry.h"
#include "mng_core/e_type_qry.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void free_dir(E **dir_buf, size_t ent_cnt) {
  if (!dir_buf)
    return;

  for (size_t i = 0; i < ent_cnt; i++) {
    if (dir_buf[i]) {
      free(dir_buf[i]->ent_nm);
      free(dir_buf[i]);
    }
  }

  if (dir_buf[ent_cnt]) {
    free(dir_buf[ent_cnt]->ent_nm);
    free(dir_buf[ent_cnt]);
  }

  free(dir_buf);
}

E **qry_dir(const char *dir_path) {
  if (!dir_path)
    return NULL;

  size_t ent_cnt = 0;
  size_t cpty = 16;

  DIR *dir = opendir(dir_path);
  if (!dir)
    return NULL;

  E **dir_buf = calloc(cpty, sizeof(*dir_buf));
  if (!dir_buf)
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

    if (ent_cnt >= cpty - 1) {
      if (cpty > SIZE_MAX / 2)
        goto err;

      size_t new_cpty = cpty * 2;

      if (new_cpty > SIZE_MAX / sizeof(*dir_buf))
        goto err;

      E **tmp = realloc(dir_buf, new_cpty * sizeof(*dir_buf));
      if (!tmp)
        goto err;

      dir_buf = tmp;

      memset(dir_buf + cpty, 0, (new_cpty - cpty) * sizeof(*dir_buf));

      cpty = new_cpty;
    }

    E *ent = calloc(1, sizeof(*ent));
    if (!ent)
      goto err;

    dir_buf[ent_cnt] = ent;

    size_t name_len = strlen(e->d_name);

    ent->ent_nm = malloc(name_len + 1);
    if (!ent->ent_nm)
      goto err;

    memcpy(ent->ent_nm, e->d_name, name_len + 1);

    size_t path_len = strlen(dir_path);
    bool has_slash = path_len > 0 && dir_path[path_len - 1] == '/';

    if (path_len > SIZE_MAX - name_len - 2)
      goto err;

    size_t full_len = path_len + name_len + (has_slash ? 1 : 2);

    char *path = malloc(full_len);
    if (!path)
      goto err;

    snprintf(path, full_len, "%s%s%s", dir_path, has_slash ? "" : "/",
             e->d_name);

    char *type = get_ent_type(path);
    free(path);

    if (!type)
      goto err;

    size_t type_len = strlen(type);

    if (type_len >= sizeof(ent->ent_type))
      goto err;

    memcpy(ent->ent_type, type, type_len + 1);

    ent_cnt++;
  }

  // Allocate sentinel.
  dir_buf[ent_cnt] = calloc(1, sizeof(E));
  if (!dir_buf[ent_cnt])
    goto err;

  dir_buf[ent_cnt]->ent_nm = calloc(1, 1);
  if (!dir_buf[ent_cnt]->ent_nm)
    goto err;

  {
    int cl = closedir(dir);
    dir = NULL;

    if (cl == -1)
      goto err;
  }

  return dir_buf;

err:
  if (dir)
    closedir(dir);

  free_dir(dir_buf,
           ent_cnt + (dir_buf && ent_cnt < cpty && dir_buf[ent_cnt] != NULL));

  return NULL;
}
