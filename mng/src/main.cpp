#include "mng/args.h"
#include "mng/dir_pnl.hpp"
#include "mng/dtls_pnl.hpp"
#include <filesystem>

#include <locale.h>
#include <ncurses.h>

static void upd_lyt(mng::DirPnl &dir_pnl, mng::DtlsPnl &dtls_pnl) {
  if (dtls_pnl.get_vis()) {
    dir_pnl.resize(0, 0, COLS - (COLS / 3), LINES);
    dtls_pnl.resize(COLS - (COLS / 3), 0, COLS / 3, LINES);
    dtls_pnl.prt();
  } else
    dir_pnl.resize(0, 0, COLS, LINES);

  dir_pnl.prt();
}

int main(int argc, char **argv) {
  Args args;
  parse_args(argc, argv, &args);
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  refresh();

  {
    mng::DirPnl dir_pnl(args.init_path != NULL
                            ? args.init_path
                            : std::filesystem::current_path());
    mng::DtlsPnl dtls_pnl(dir_pnl.get_curr_path());
    dtls_pnl.set_vis(false);

    upd_lyt(dir_pnl, dtls_pnl);
    mng::Pnl::rfsh();

    int key;
    while ((key = getch()) != 'q') {
      switch (key) {
      case 'i':
        if (dtls_pnl.get_vis())
          dtls_pnl.set_vis(false);
        else
          dtls_pnl.set_vis(true);
        break;
      default:
        dir_pnl.hndl_in(key);
        break;
      }
      dtls_pnl.set_ent(dir_pnl.get_curr_path());
      upd_lyt(dir_pnl, dtls_pnl);
      mng::Pnl::rfsh();
    }
  }
  endwin();
  return 0;
}
