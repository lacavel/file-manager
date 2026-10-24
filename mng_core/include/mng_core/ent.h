#pragma once

#include <sys/types.h>

typedef struct {
  char ent_sz[16];
  char ent_type[8];
  char *ent_nm;
} Ent;

ssize_t pop_dir(Ent **out_dir, const char *dir_path);
void free_dir(Ent *dir, size_t ent_cnt);
