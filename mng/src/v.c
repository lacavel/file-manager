#include "mng_core/e_perms_qry.h"
#include "mng_core/e_sz_qry.h"
#include "mng_core/e_tm_qry.h"
#include "mng_core/e_utils.h"

#include "v.h"
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>

static int sts_pnl_wd() {
  int wd = COLS / 3;
  if (wd < 32)
    wd = 32;
  if (wd > COLS - 16)
    wd = COLS - 16;
  return wd;
}

void draw_sts_pnl(const Ui *ui, const AppState *st) {
  if (!st->sts_visible || !ui->sts_pnl || st->ent_cnt == 0)
    return;

  werase(ui->sts_pnl);
  box(ui->sts_pnl, 0, 0);

  int max_wd = COLS / 3 - 4;
  if (max_wd < 16)
    max_wd = 16;

  char ent_path[PATH_MAX];
  nav_down(ent_path, st->path, st->dir_buf[st->sel]->ent_nm);

  int row = 1;

  mvwprintw(ui->sts_pnl, 0, 2, "%.*s", max_wd, st->dir_buf[st->sel]->ent_nm);

  mvwprintw(ui->sts_pnl, row++, 2, "Times:");
  char tm_buf[128];
  const char *tm_fmt = "%d/%m/%Y %H:%M";

  if (get_ent_mod_tm(tm_buf, ent_path, tm_fmt) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Modified: %.*s", max_wd, tm_buf);

  if (get_ent_acs_tm(tm_buf, ent_path, tm_fmt) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Accessed: %.*s", max_wd, tm_buf);

  if (get_ent_chg_tm(tm_buf, ent_path, tm_fmt) != 0)
    mvwprintw(ui->sts_pnl, row++, 2, "Changed: %.*s", max_wd, tm_buf);

  row++;
  mvwprintw(ui->sts_pnl, row++, 2, "Ownership:");
  char own_buf[64], grp_buf[64], perms_buf[12];
  if (get_ent_own(own_buf, ent_path) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Owner: %.*s", max_wd, own_buf);
  if (get_ent_grp(grp_buf, ent_path) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Group: %.*s", max_wd, grp_buf);

  row++;
  mvwprintw(ui->sts_pnl, row++, 2, "Permissions:");
  if (get_ent_own_perms(perms_buf, ent_path) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Owner: %s", perms_buf);
  if (get_ent_grp_perms(perms_buf, ent_path) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Group: %s", perms_buf);
  if (get_ent_oth_perms(perms_buf, ent_path) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Other: %s", perms_buf);

  row++;
  mvwprintw(ui->sts_pnl, row++, 2, "Size:");
  char sz_buf[64], sz_buf_fmt[64];
  if (get_ent_sz(sz_buf, ent_path, false) != -1)
    mvwprintw(ui->sts_pnl, row++, 2, "Bytes: %.*s", max_wd, sz_buf);
  if (get_ent_sz(sz_buf_fmt, ent_path, true) != -1 &&
      strcmp(sz_buf, sz_buf_fmt) != 0)
    mvwprintw(ui->sts_pnl, row++, 2, "Formatted: %s", sz_buf_fmt);

  wrefresh(ui->sts_pnl);
}

void v_init(Ui *ui, const AppState *st) {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  curs_set(0);

  ui->dir_pnl = NULL;
  ui->sts_pnl = NULL;
  ui->ver_pnl = NULL;
  ui->col_wd = ui->max_rows = ui->max_cols = 0;

  v_sync(ui, st);
  ui->ver_pnl = newwin(1, COLS, LINES - 1, 0);
  keypad(ui->dir_pnl, TRUE);
}

void v_sync(Ui *ui, const AppState *st) {
  unsigned int col_wd = 0;
  for (unsigned int i = 0; i < st->ent_cnt; i++) {
    unsigned int len = strlen(st->dir_buf[i]->ent_nm);
    if (len > col_wd)
      col_wd = len;
  }
  ui->col_wd = col_wd + 2;

  int dir_wd = st->sts_visible ? COLS - sts_pnl_wd() : COLS;

  if (!ui->dir_pnl)
    ui->dir_pnl = newwin(LINES - 4, dir_wd, 2, 0);
  else
    wresize(ui->dir_pnl, LINES - 4, dir_wd);

  if (st->sts_visible) {
    if (!ui->sts_pnl)
      ui->sts_pnl = newwin(LINES - 4, sts_pnl_wd(), 2, (COLS - sts_pnl_wd()));
    else {
      wresize(ui->sts_pnl, LINES - 4, sts_pnl_wd());
      mvwin(ui->sts_pnl, 2, (COLS - sts_pnl_wd()));
    }
  }
  const char *ver = "mng v1.0.0";
  mvwprintw(ui->ver_pnl, 0, COLS - (int)strlen(ver) - 1, "%s", ver);

  ui->max_rows = getmaxy(ui->dir_pnl) - 2;
  ui->max_cols = (getmaxx(ui->dir_pnl) - 2) / ui->col_wd;

  clear();
  refresh();
}

void v_draw(const Ui *ui, const AppState *st) {
  werase(ui->dir_pnl);
  box(ui->dir_pnl, 0, 0);
  mvwprintw(ui->dir_pnl, 0, 2, "%s", st->path);
  for (unsigned int idx = 0;
       idx < st->ent_cnt && idx < ui->max_rows * ui->max_cols; idx++) {
    unsigned int row = 1 + (idx % ui->max_rows);
    unsigned int col = idx / ui->max_rows;
    if (idx == st->sel)
      wattron(ui->dir_pnl, A_REVERSE);
    mvwprintw(ui->dir_pnl, row, 2 + (col * ui->col_wd), "%s%s",
              st->dir_buf[idx]->ent_type, st->dir_buf[idx]->ent_nm);
    wattroff(ui->dir_pnl, A_REVERSE);
  }
  mvwprintw(ui->dir_pnl, ui->max_rows + 1, 2, "%u entries", st->ent_cnt);

  if (st->sts_visible && ui->sts_pnl && st->ent_cnt > 0)
    draw_sts_pnl(ui, st);

  wrefresh(ui->sts_pnl);
  wrefresh(ui->dir_pnl);
  wrefresh(ui->ver_pnl);
}

void v_destroy(Ui *ui) {
  if (ui->sts_pnl)
    delwin(ui->sts_pnl);
  if (ui->dir_pnl)
    delwin(ui->dir_pnl);
  endwin();
}
