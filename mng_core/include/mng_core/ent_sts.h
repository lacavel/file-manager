#pragma once
#include <stddef.h>

size_t get_ent_own_perms(char *own_perms, const char *ent_path);
size_t get_ent_grp_perms(char *grp_perms, const char *ent_path);
size_t get_ent_oth_perms(char *oth_perms, const char *ent_path);
size_t get_ent_own(char *buf, const size_t buf_sz, const char *ent_path);
size_t get_ent_grp(char *buf, const size_t buf_sz, const char *ent_path);
size_t get_ent_sz(char *buf, const size_t buf_sz, const char *ent_path);
size_t get_ent_acs_tm(char *buf, const size_t buf_sz, const char *ent_path,
                      const char *tm_fmt);
size_t get_ent_mod_tm(char *buf, const size_t buf_sz, const char *ent_path,
                      const char *tm_fmt);
size_t get_ent_chg_tm(char *buf, const size_t buf_sz, const char *ent_path,
                      const char *tm_fmt);
