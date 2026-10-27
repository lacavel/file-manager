#include "mng_core/e_qry.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

static void init_scrn() {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);
}

int main(int argc, char **argv) {
  init_scrn();

  WINDOW *win = newwin(LINES - 2, COLS, 1, 0);
  box(win, 0, 0);

  const char *path = (argc == 2 && argv[1] != NULL) ? argv[1] : ".";

  size_t max_nm_len = 0;
  char **dir;
  ssize_t e_cnt = qry_dir(&dir, path, &max_nm_len);
  if (e_cnt < 0)
    return 1;
  int max_y = getmaxy(win);
  int max_x = getmaxx(win);

  int col_width = max_nm_len;

  for (size_t i = 0; i < (size_t)e_cnt; i++) {
    int y = (i % (max_y - 2)) + 1;

    int col_index = i / (max_y - 2);
    int x = 2 + (col_index * col_width);

    if (x >= max_x - 5)
      break;

    mvwprintw(win, y, x, "%s", dir[i]);
  }

  mvwprintw(win, 0, 2, "%s", path);
  mvwprintw(win, max_y - 1, 2, "%u entries", (unsigned int)e_cnt);
  refresh();
  wrefresh(win);

  int ch;
  while ((ch = wgetch(win)) != 'q') {
  }

  delwin(win);
  free_dir(dir, e_cnt);

  endwin();

  return 0;
}
