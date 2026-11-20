#include "ctrl.hpp"
#include <ncurses.h>

mng::Ctrl::Ctrl() {
  int nav_height = 3;
  int main_height = LINES - nav_height;
  int dir_width = COLS * 2 / 3;
  int stats_width = COLS - dir_width;

  dir_pnl = std::make_unique<DirPnl>(0, nav_height, dir_width, main_height);
  dir_pnl->set_dir_path(".");
  dir_pnl->rfsh();
}

void mng::Ctrl::run() {
  while (running) {
    render();
    int key = getch();
    handle_in(key);
  }
}

void mng::Ctrl::handle_in(int key) {
  switch (key) {
  case 'q':
    running = false;
    break;

  case KEY_UP:
  case KEY_DOWN:
  case KEY_LEFT:
  case KEY_RIGHT:
  case '\n':
    dir_panel->handle_in(key);

    // Update stats panel based on new selection
    if (auto selected = dir_panel->get_selected()) {
      stats_panel->set_target(*selected);
    }
    break;

  case '/':
    // Activate nav bar for search
    nav_bar->activate();
    break;
  }
}

void mng::Controller::render() {
  clear();
  nav_bar->prt();
  dir_panel->prt();
  stats_panel->prt();

  nav_bar->rfsh();
  dir_panel->rfsh();
  stats_panel->rfsh();
  refresh(); // Refresh stdscr
}
