#include "mng_core/ent.h"
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

  Ent *dir = NULL;
  const char *path = (argc == 2 && argv[1] != NULL) ? argv[1] : ".";

  ssize_t ent_cnt_signed = pop_dir(&dir, path);
  if (ent_cnt_signed < 0 || dir == NULL) {
    endwin();
    fprintf(stderr, "Error: Failed to read directory '%s'\n", path);
    return 1;
  }
  size_t ent_cnt = (size_t)ent_cnt_signed;

  int max_y = getmaxy(win);
  int max_x = getmaxx(win);

  int rows_per_col = max_y - 2;
  if (rows_per_col < 1)
    rows_per_col = 1;

  int col_width = max_x / 2;

  for (size_t i = 0; i < ent_cnt; i++) {
    int y = (i % rows_per_col) + 1;

    int col_index = i / rows_per_col;
    int x = 2 + (col_index * col_width);

    if (x >= max_x - 5)
      break;

    char line_buf[128];
    snprintf(line_buf, sizeof(line_buf), "%s %s (%s)", dir[i].ent_type,
             dir[i].ent_nm, dir[i].ent_sz);

    mvwprintw(win, y, x, "%-*.*s", col_width - 2, col_width - 2, line_buf);
  }

  mvwprintw(win, 0, 2, "%s", path);
  mvwprintw(win, max_y - 1, 2, "%u entries", (unsigned int)ent_cnt);
  refresh();
  wrefresh(win);

  int ch;
  while ((ch = wgetch(win)) != 'q') {
  }

  delwin(win);
  free_dir(dir, ent_cnt);

  endwin();

  return 0;
}
