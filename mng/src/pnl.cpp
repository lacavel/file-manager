#include "pnl.hpp"
#include <ncurses.h>

struct mng::PnlImpl {
  WINDOW *pnl;
};

mng::Pnl::Pnl(unsigned int x, unsigned int y, unsigned int wd, unsigned int ht)
    : pimpl(std::make_unique<PnlImpl>()) {
  pimpl->pnl = newwin(ht, wd, y, x);
  if (pimpl->pnl)
    keypad(pimpl->pnl, TRUE);
}

mng::Pnl::~Pnl() {
  if (pimpl && pimpl->pnl)
    delwin(pimpl->pnl);
}

mng::Pnl::Pnl(Pnl &&oth) noexcept = default;
mng::Pnl &mng::Pnl::operator=(Pnl &&oth) noexcept = default;

void *mng::Pnl::get_hndl() const {
  return pimpl ? static_cast<void *>(pimpl->pnl) : nullptr;
}

void mng::Pnl::resize(unsigned int x, unsigned int y, unsigned int wd,
                      unsigned int ht) {
  if (!pimpl || !pimpl->pnl)
    return;
  wresize(pimpl->pnl, ht, wd);
  mvwin(pimpl->pnl, y, x);
}

void mng::Pnl::rfsh() {
  if (pimpl && pimpl->pnl)
    wrefresh(pimpl->pnl);
}
