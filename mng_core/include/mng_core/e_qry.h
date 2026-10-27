#pragma once

#include <sys/types.h>

ssize_t qry_dir(char ***dir_buf, const char *dir_path, size_t *max_ent_nm_len);
void free_dir(char **dir_buf, const size_t ent_cnt);
