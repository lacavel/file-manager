#include "dir_pnl.hpp"
#include "mng_core/e_qry.h"
#include "mng_core/e_utils.h"

mng::DirPnl::DirPnl(unsigned int x, unsigned int y, unsigned int wd,
                    unsigned int ht)
    : Pnl(x, y, wd, ht) {}

void mng::DirPnl::prt() {
  if (!this->dir_path)
    return;
}
