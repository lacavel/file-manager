#include "mng_core/e_perms_qry.h"
#include "mng_core/e_qry.h"
#include "mng_core/e_sz_qry.h"
#include "mng_core/e_tm_qry.h"

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

static inline int get_sts_pnl_wd() { return LINES / 4; }

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

static void dir_pnl_nav(DirPnl *p, int key) {
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
    dir_pnl_nav(&pnl, ch);
    pnt_dir_pnl(&pnl);
    wrefresh(pnl.win);
  }

  endwin();
  return 0;
}
