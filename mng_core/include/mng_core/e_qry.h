#pragma once
#include <stddef.h>

typedef struct {
  char ent_type[8];
  char *ent_nm;
} E;

E **qry_dir(const char *dir_path);
void free_dir(E **dir_buf, size_t ent_cnt);
