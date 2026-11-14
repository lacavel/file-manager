#pragma once

#include <sys/types.h>

ssize_t pop_dir(char ***dir_buf, const char *dir_path);
char **qry_dir(const char *dir_path);
void free_dir(char **dir_buf, const size_t ent_cnt);
