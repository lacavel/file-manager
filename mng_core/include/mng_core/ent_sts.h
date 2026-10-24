#pragma once

// Caller must allocate >= 16 bytes.
int get_ent_own_perms(char *own_perms_buf, const char *ent_path);
int get_ent_grp_perms(char *grp_perms_buf, const char *ent_path);
int get_ent_oth_perms(char *oth_perms_buf, const char *ent_path);

// Caller must allocate >= 256 bytes.
int get_ent_own(char *own_buf, const char *ent_path);
int get_ent_grp(char *grp_buf, const char *ent_path);

// Caller must allocate >= 24 bytes.
int get_ent_sz(char *sz_buf, const char *ent_path);

// Caller may allocate >= 64 bytes
int get_ent_acs_tm(char *acs_tm_buf, const char *ent_path, const char *tm_fmt);
int get_ent_mod_tm(char *mod_tm_buf, const char *ent_path, const char *tm_fmt);
int get_ent_chg_tm(char *chg_tm_buf, const char *ent_path, const char *tm_fmt);
