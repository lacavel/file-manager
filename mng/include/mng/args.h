#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char *init_path;
  bool ver;
} Args;

bool parse_args(int argc, char **argv, Args *args);

#ifdef __cplusplus
}
#endif
