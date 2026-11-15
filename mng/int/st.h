#pragma once
#include "mng_core/e_qry.h"
#include <limits.h>
#include <stdbool.h>

typedef struct {
  char path[PATH_MAX];
  E **dir_buf;
  unsigned int ent_cnt;
  unsigned int sel;
  bool sts_visible;
} AppState;
