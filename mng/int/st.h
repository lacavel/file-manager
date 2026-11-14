#pragma once
#include <limits.h>
#include <stdbool.h>

typedef struct {
  char path[PATH_MAX];
  char **dir_buf;
  unsigned int ent_cnt;
  unsigned int sel;
  bool sts_visible;
} AppState;
