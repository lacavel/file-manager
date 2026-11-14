#pragma once

void get_abs_path(char *path_buf, const char *ent_path);
int nav_up(char *path_buf, const char *path);
int nav_down(char *path_buf, const char *path, const char *ent_nm);
