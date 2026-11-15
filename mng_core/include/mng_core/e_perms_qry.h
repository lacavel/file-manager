#pragma once
#include <stdbool.h>

int get_ent_own_perms(char *own_perms_buf, const char *ent_path);
int get_ent_grp_perms(char *grp_perms_buf, const char *ent_path);
int get_ent_oth_perms(char *oth_perms_buf, const char *ent_path);

int get_ent_own(char *own_buf, const char *ent_path);
int get_ent_grp(char *grp_buf, const char *ent_path);
