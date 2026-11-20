#include "dir_pnl.hpp"
#include <filesystem>

#include <locale.h>
#include <ncurses.h>

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  refresh();

  mng::DirPnl dir_pnl(0, 0, COLS, LINES - 1);
  dir_pnl.set_dir(argc == 2 && argv[1] ? argv[1]
                                       : std::filesystem::current_path());
  dir_pnl.prt();
  dir_pnl.rfsh();

  int key;
  while ((key = getch()) != 'q') {
    dir_pnl.hndl_in(key);
    dir_pnl.prt();
    dir_pnl.rfsh();
  }

  endwin();
  return 0;
}
