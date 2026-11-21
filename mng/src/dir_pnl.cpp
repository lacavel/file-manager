#include "mng/dir_pnl.hpp"
#include <ncurses.h>

mng::DirPnl::DirPnl(std::filesystem::path dir_path) : Pnl(0, 0, COLS, LINES) {
  this->dir_path = dir_path;
  this->dir_buf = mng_core::qry_dir(dir_path);
  this->cur_idx = 0;
}

void mng::DirPnl::prt() {
  auto *dir_pnl = static_cast<WINDOW *>(get_hndl());
  if (!dir_pnl)
    return;
  werase(dir_pnl);
  box(dir_pnl, 0, 0);
  mvwprintw(dir_pnl, 0, 2, "%s", dir_path.c_str());
  unsigned short row = 1, col = 2, col_wd = 0;
  unsigned int wd = getmaxx(dir_pnl) - 2, ht = getmaxy(dir_pnl) - 2;
  for (const auto &ent : dir_buf)
    col_wd =
        (ent.ent_nm.length() + 4) > col_wd ? ent.ent_nm.length() + 4 : col_wd;

  for (unsigned int idx = 0; idx < dir_buf.size(); idx++) {
    if (row > ht) {
      row = 1;
      col = (col + col_wd < (wd - 2)) ? col + col_wd : col;
    }
    if (idx == cur_idx)
      wattron(dir_pnl, A_REVERSE);

    mvwprintw(dir_pnl, row, col, "%s%s", dir_buf[idx].ent_type.c_str(),
              dir_buf[idx].ent_nm.c_str());
    if (idx == cur_idx)
      wattroff(dir_pnl, A_REVERSE);

    row++;
  }
  mvwprintw(dir_pnl, ht + 1, 2, "%lu entries", dir_buf.size());
}

void mng::DirPnl::hndl_in(int key) {
  switch (key) {
  case KEY_UP:
  case 'k': {
    if (cur_idx > 0)
      cur_idx--;
    break;
  }
  case KEY_DOWN:
  case 'j': {
    if (cur_idx < dir_buf.size() - 1)
      cur_idx++;
    break;
  }
  case KEY_ENTER:
  case 'l': {
    if (dir_buf.empty() || cur_idx >= dir_buf.size())
      break;

    std::filesystem::path next = dir_path / dir_buf[cur_idx].ent_nm;
    std::error_code err;
    if (!std::filesystem::is_directory(next, err))
      break;

    auto canon = std::filesystem::canonical(next, err);
    if (err)
      break;

    set_dir_path(canon);
    break;
  }
  case 'h':
  case KEY_BACKSPACE: {
    set_dir_path(dir_path.parent_path());
    break;
  } break;
  }
}

void mng::DirPnl::set_dir_path(std::filesystem::path dir_path) {
  this->dir_buf = mng_core::qry_dir(dir_path);
  this->dir_path = dir_path;
  this->cur_idx = 0;
}

std::filesystem::path mng::DirPnl::get_curr_path() const {
  if (dir_buf.empty() || cur_idx > dir_buf.size())
    return dir_path;
  return dir_path / dir_buf[cur_idx].ent_nm;
}
