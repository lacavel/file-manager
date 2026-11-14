#include "mng_core/e_perms_qry.h"
#include "mng_core/e_qry.h"
#include "mng_core/e_sz_qry.h"
#include "mng_core/e_tm_qry.h"
#include "mng_core/e_type_qry.h"
#include "mng_core/e_utils.h"
#include <limits.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

typedef struct {
  WINDOW *win;
  char **dir_buf;
  const char *path;
  unsigned int ent_cnt;
  unsigned int sel;
  unsigned int col_wd;
  unsigned int max_rows;
  unsigned int max_cols;
} DirPnl;

static bool sts_pnl_vis = false;
static WINDOW *sts_win = NULL;

static inline int get_sts_pnl_wd() {
  int wd = COLS / 4;
  if (wd < 32)
    wd = 32;
  if (wd > COLS - 16)
    wd = COLS - 16;
  return wd;
}

static unsigned int get_ent_cnt(char **dir_buf, unsigned int *col_wd) {
  unsigned int ent_cnt = 0, ent_nm_len = 0;
  while (dir_buf[ent_cnt][0] != '\0') {
    ent_nm_len = strlen(dir_buf[ent_cnt]);
    if (ent_nm_len > *col_wd)
      *col_wd = ent_nm_len;
    ent_cnt++;
  }
  return ent_cnt;
}

static void init_dir_pnl(DirPnl *p, const char *path, char **dir_buf) {
  *p = (DirPnl){
      .win = newwin(LINES, sts_pnl_vis ? COLS - get_sts_pnl_wd() : COLS, 0, 0),
      .path = path,
      .dir_buf = dir_buf,
  };
  keypad(p->win, TRUE);
  p->ent_cnt = get_ent_cnt(dir_buf, &p->col_wd);
  p->col_wd += 2;
  p->max_rows = getmaxy(p->win) - 2;
  p->max_cols = (getmaxx(p->win) - 2) / p->col_wd;
}

static void toggle_sts_pnl(DirPnl *p) {
  sts_pnl_vis = !sts_pnl_vis;
  if (sts_pnl_vis) {
    if (!sts_win)
      sts_win = newwin(LINES, get_sts_pnl_wd(), 0, COLS - get_sts_pnl_wd());

    wresize(p->win, LINES, COLS - get_sts_pnl_wd());
  } else
    wresize(p->win, LINES, COLS);

  p->max_rows = getmaxy(p->win) - 2;
  p->max_cols = (getmaxx(p->win) - 2) / p->col_wd;

  clear();
  refresh();
}

static void pnt_dir_pnl(const DirPnl *p) {
  werase(p->win);
  box(p->win, 0, 0);
  mvwprintw(p->win, 0, 2, "%s", p->path);
  for (unsigned int idx = 0;
       idx < p->ent_cnt && idx < p->max_rows * p->max_cols; idx++) {
    unsigned int row = 1 + (idx % p->max_rows);
    unsigned int col = idx / p->max_rows;
    if (idx == p->sel)
      wattron(p->win, A_REVERSE);
    mvwprintw(p->win, row, 2 + (col * p->col_wd), "%s", p->dir_buf[idx]);
    wattroff(p->win, A_REVERSE);
  }
  mvwprintw(p->win, p->max_rows + 1, 2, "%u entries", p->ent_cnt);
}

static void pnt_sts_pnl(DirPnl *p) {
  if (!sts_pnl_vis || !sts_win || p->ent_cnt == 0)
    return;

  werase(sts_win);
  box(sts_win, 0, 0);

  int max_wd = get_sts_pnl_wd() - 4;
  if (max_wd < 0)
    max_wd = 0;

  char ent_path[PATH_MAX];
  nav_down(ent_path, p->path, p->dir_buf[p->sel]);

  int row = 1;

  mvwprintw(sts_win, 0, 2, "%.*s", max_wd, p->dir_buf[p->sel]);

  mvwprintw(sts_win, row++, 2, "Times:");
  char tm_buf[128];
  const char *tm_fmt = "%d/%m/%Y %H:%M";

  if (get_ent_mod_tm(tm_buf, ent_path, tm_fmt) != -1)
    mvwprintw(sts_win, row++, 2, "Modified: %.*s", max_wd, tm_buf);

  if (get_ent_acs_tm(tm_buf, ent_path, tm_fmt) != -1)
    mvwprintw(sts_win, row++, 2, "Accessed: %.*s", max_wd, tm_buf);

  if (get_ent_chg_tm(tm_buf, ent_path, tm_fmt) != 0)
    mvwprintw(sts_win, row++, 2, "Changed: %.*s", max_wd, tm_buf);

  row++;
  mvwprintw(sts_win, row++, 2, "Ownership:");
  char perms_buf[12], own_buf[64], grp_buf[64];
  if (get_ent_own(own_buf, ent_path) != -1)
    mvwprintw(sts_win, row++, 2, "Owner: %.*s", max_wd, own_buf);
  if (get_ent_grp(grp_buf, ent_path) != -1)
    mvwprintw(sts_win, row++, 2, "Group: %.*s", max_wd, grp_buf);

  row++;
  mvwprintw(sts_win, row++, 2, "Permissions:");
  if (get_ent_own_perms(perms_buf, ent_path) != -1)
    mvwprintw(sts_win, row++, 2, "Owner: %s", perms_buf);
  if (get_ent_grp_perms(perms_buf, ent_path) != -1)
    mvwprintw(sts_win, row++, 2, "Group: %s", perms_buf);
  if (get_ent_oth_perms(perms_buf, ent_path) != -1)
    mvwprintw(sts_win, row++, 2, "Other: %s", perms_buf);

  row++;
  mvwprintw(sts_win, row++, 2, "Size:");
  char sz_buf[64], sz_buf_fmt[64];
  if (get_ent_sz(sz_buf, ent_path, false) != -1)
    mvwprintw(sts_win, row++, 2, "Bytes: %.*s", max_wd, sz_buf);
  if (get_ent_sz(sz_buf_fmt, ent_path, true) != -1 &&
      strcmp(sz_buf, sz_buf_fmt) != 0)
    mvwprintw(sts_win, row++, 2, "Formatted: %s", sz_buf_fmt);

  wrefresh(sts_win);
}

static void dir_pnl_nav(DirPnl *p, int key) {
  if (p->ent_cnt == 0 || p->max_rows == 0)
    return;

  unsigned int row = p->sel % p->max_rows;
  unsigned int col = p->sel / p->max_rows;
  switch (key) {
  case 'k':
  case KEY_UP:
    if (row > 0)
      p->sel--;
    break;
  case 'j':
  case KEY_DOWN:
    if (row < p->max_rows - 1 && p->sel < p->ent_cnt - 1)
      p->sel++;
    break;
  case 'h':
  case KEY_LEFT:
    if (col > 0) {
      p->sel = (col - 1) * p->max_rows + row;
      if (p->sel >= p->ent_cnt)
        p->sel = p->ent_cnt - 1;
    }
    break;
  case 'l':
  case KEY_RIGHT:
    if (col < p->max_cols - 1) {
      size_t new_idx = (col + 1) * p->max_rows + row;
      if (new_idx < p->ent_cnt)
        p->sel = new_idx;
    }
    break;
  default:
    break;
  }
}

static void init_scr() {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  curs_set(0);
}

int main(int argc, char **argv) {
  char init_path[PATH_MAX];
  get_abs_path(init_path, (argc == 2 && argv[1]) ? argv[1] : ".");
  init_scr();

  char **dir_buf = qry_dir(init_path);
  if (!dir_buf) {
    endwin();
    return 1;
  }

  DirPnl pnl;
  init_dir_pnl(&pnl, init_path, dir_buf);

  pnt_dir_pnl(&pnl);
  wrefresh(pnl.win);

  int ch;
  while ((ch = wgetch(pnl.win)) != 'q') {
    if (ch == 'i')
      toggle_sts_pnl(&pnl);
    else
      dir_pnl_nav(&pnl, ch);

    pnt_dir_pnl(&pnl);
    pnt_sts_pnl(&pnl);
    wrefresh(pnl.win);
  }

  if (sts_win)
    delwin(sts_win);

  endwin();
  free_dir(dir_buf, pnl.ent_cnt);
  return 0;
}
