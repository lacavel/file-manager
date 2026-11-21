#include "mng/pnl.hpp"
#include <ncurses.h>
#include <panel.h>

struct mng::PnlImpl {
  WINDOW *wnd = nullptr;
  PANEL *pnl = nullptr;
};

mng::Pnl::Pnl(unsigned int x, unsigned int y, unsigned int wd, unsigned int ht)
    : pimpl(std::make_unique<PnlImpl>()) {
  pimpl->wnd = newwin(ht, wd, y, x);
  if (pimpl->wnd) {
    keypad(pimpl->wnd, TRUE);
    pimpl->pnl = new_panel(pimpl->wnd);
  }
}

mng::Pnl::~Pnl() {
  if (pimpl && pimpl->pnl)
    del_panel(pimpl->pnl);
  if (pimpl && pimpl->wnd)
    delwin(pimpl->wnd);
}

mng::Pnl::Pnl(Pnl &&oth) noexcept = default;
mng::Pnl &mng::Pnl::operator=(Pnl &&oth) noexcept = default;

void *mng::Pnl::get_hndl() const {
  return pimpl ? static_cast<void *>(pimpl->wnd) : nullptr;
}

void mng::Pnl::resize(unsigned int x, unsigned int y, unsigned int wd,
                      unsigned int ht) {
  if (!pimpl || !pimpl->wnd)
    return;
  wresize(pimpl->wnd, ht, wd);
  if (pimpl->pnl) {
    move_panel(pimpl->pnl, y, x);
    replace_panel(pimpl->pnl, pimpl->wnd);
  } else
    mvwin(pimpl->wnd, y, x);
  touchwin(pimpl->wnd);
}

void mng::Pnl::set_vis(bool vis) {
  if (!pimpl || !pimpl->pnl)
    return;
  if (vis)
    show_panel(pimpl->pnl);
  else
    hide_panel(pimpl->pnl);
}

bool mng::Pnl::get_vis() { return pimpl->pnl && !panel_hidden(pimpl->pnl); }

void mng::Pnl::rfsh() {
  update_panels();
  doupdate();
}
