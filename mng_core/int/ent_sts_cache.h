#pragma once
#include <stdbool.h>
#include <sys/stat.h>

const struct stat *get_ent_sts(const char *ent_path, bool flw);
