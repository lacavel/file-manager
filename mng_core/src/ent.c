#include "mng_core/ent.h"
#include "ent_sts_cache.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static char *get_path(const char *dir_path, const char *ent_nm) {
  size_t dir_len = strlen(dir_path), nm_len = strlen(ent_nm),
         sep_len = dir_len > 0 && dir_path[dir_len - 1] == '/' ? 0 : 1;

  if (dir_len > SIZE_MAX - sep_len || dir_len + sep_len > SIZE_MAX - nm_len - 1)
    return NULL;

  char *path = NULL;
  if (!(path = malloc(dir_len + sep_len + nm_len + 1)))
    return NULL;

  memcpy(path, dir_path, dir_len);

  if (sep_len)
    path[dir_len++] = '/';

  memcpy(path + dir_len, ent_nm, nm_len + 1);
  return path;
}

static char *fmt_ent_type(const char *ent_path) {
  const struct stat *sts;
  if (!(sts = get_ent_sts(ent_path)))
    return NULL;

  switch (sts->st_mode & S_IFMT) {
  case S_IFREG:
    return "";
  case S_IFDIR:
    return "";
  case S_IFLNK: {
    struct stat sts;
    if (stat(ent_path, &sts) == -1)
      return NULL;

    if (S_ISDIR(sts.st_mode))
      return "";
    else
      return "";
  }
  case S_IFBLK:
  case S_IFCHR:
  case S_IFIFO:
  case S_IFSOCK:
  default:
    return "";
  }
}

ssize_t pop_dir(Ent **out_dir, const char *dir_path) {
  if (!out_dir || !dir_path)
    return -1;

  *out_dir = NULL;

  DIR *d = opendir(dir_path);
  if (!d)
    return -1;

  size_t ent_cnt = 0, cpty = 16;
  Ent *dir = NULL;

  if (!(dir = malloc(cpty * sizeof(*dir))))
    goto err;

  struct dirent *e;

  for (;;) {
    errno = 0;
    e = readdir(d);

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
      if (cpty > SIZE_MAX / sizeof(*dir))
        goto err;

      Ent *tmp = NULL;
      if (!(tmp = realloc(dir, cpty * sizeof(*dir))))
        goto err;

      dir = tmp;
    }

    const size_t nm_len = strlen(e->d_name);
    dir[ent_cnt].ent_nm = malloc(nm_len + 1);
    if (!dir[ent_cnt].ent_nm)
      goto err;
    memcpy(dir[ent_cnt].ent_nm, e->d_name, nm_len + 1);

    char *path = NULL;
    if (!(path = get_path(dir_path, e->d_name)))
      goto err;

    const char *ent_type = NULL;
    if (!(ent_type = fmt_ent_type(path)))
      continue;

    free(path);
    const size_t type_len = strlen(ent_type);
    memcpy(dir[ent_cnt].ent_type, ent_type, type_len + 1);
    dir[ent_cnt].ent_type[type_len] = '\0';

    ent_cnt++;
  }

  {
    const int cl = closedir(d);
    d = NULL;

    if (cl == -1)
      goto err;
  }

  if (ent_cnt > (size_t)SSIZE_MAX)
    goto err;

  *out_dir = dir;
  return (ssize_t)ent_cnt;

err:
  if (d)
    closedir(d);

  free_dir(dir, ent_cnt);
  return -1;
}

void free_dir(Ent *dir, size_t ent_cnt) {
  if (!dir)
    return;

  for (size_t i = 0; i < ent_cnt; i++)
    free(dir[i].ent_nm);

  free(dir);
}
