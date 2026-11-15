#include "cntlr.h"
#include "mng_core/e_qry.h"
#include "mng_core/e_utils.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static unsigned int cnt_e(E **dir_buf) {
  unsigned int cnt = 0;
  while (dir_buf[cnt]->ent_nm[0] != '\0')
    cnt++;
  return cnt;
}

static bool load_dir(AppState *st, const char *path) {
  E **buf = qry_dir(path);
  if (!buf)
    return false;

  free_dir(st->dir_buf, st->ent_cnt);
  st->dir_buf = buf;
  snprintf(st->path, sizeof(st->path), "%s", path);
  st->ent_cnt = cnt_e(buf);
  st->sel = 0;
  return true;
}

static void open_sel(AppState *st) {
  if (st->ent_cnt == 0)
    return;

  char ent_path[PATH_MAX];
  nav_down(ent_path, st->path, st->dir_buf[st->sel]->ent_nm);

  struct stat sb;
  if (stat(ent_path, &sb) != 0 || !S_ISDIR(sb.st_mode))
    return;

  load_dir(st, ent_path);
}

static void go_up(AppState *st) {
  char parent[PATH_MAX], resolved[PATH_MAX];
  nav_down(parent, st->path, "..");
  if (!realpath(parent, resolved))
    return;

  if (strcmp(resolved, st->path) == 0)
    return;

  load_dir(st, resolved);
}

static void nav(AppState *st, int key, unsigned int max_rows,
                unsigned int max_cols) {
  if (st->ent_cnt == 0 || max_rows == 0)
    return;

  unsigned int row = st->sel % max_rows;
  unsigned int col = st->sel / max_rows;
  switch (key) {
  case 'k':
  case KEY_UP:
    if (row > 0)
      st->sel--;
    break;
  case 'j':
  case KEY_DOWN:
    if (row < max_rows - 1 && st->sel < st->ent_cnt - 1)
      st->sel++;
    break;
  case 'h':
  case KEY_LEFT:
    if (col > 0) {
      st->sel = (col - 1) * max_rows + row;
      if (st->sel >= st->ent_cnt)
        st->sel = st->ent_cnt - 1;
    }
    break;
  case 'l':
  case KEY_RIGHT:
    if (col < max_cols - 1) {
      size_t new_idx = (col + 1) * max_rows + row;
      if (new_idx < st->ent_cnt)
        st->sel = new_idx;
    }
    break;
  default:
    break;
  }
}

bool cntlr_handle(AppState *st, int key, const Ui *ui) {
  switch (key) {
  case 'q':
    return false;
  case 'i':
    st->sts_visible = !st->sts_visible;
    break;
  case 'L':
  case KEY_ENTER:
  case '\n':
  case '\r':
    open_sel(st);
    break;
  case 'H':
    go_up(st);
    break;
  case KEY_RESIZE:
    break;
  default:
    nav(st, key, ui->max_rows, ui->max_cols);
    break;
  }
  return true;
}
