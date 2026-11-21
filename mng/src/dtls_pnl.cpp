#include "mng_core/ent_perms_qry.hpp"
#include "mng_core/ent_sz_qry.hpp"
#include "mng_core/ent_ts_qry.hpp"

#include "mng/dtls_pnl.hpp"
#include <ncurses.h>

mng::DtlsPnl::DtlsPnl(std::filesystem::path ent_path)
    : Pnl(COLS - (COLS / 3), 0, COLS / 3, LINES), ent_path(ent_path) {}

void mng::DtlsPnl::prt() {
  auto *dtls_pnl = static_cast<WINDOW *>(get_hndl());
  if (!dtls_pnl)
    return;
  werase(dtls_pnl);
  box(dtls_pnl, 0, 0);
  unsigned int row = 0, left_pad = 2;
  mvwprintw(dtls_pnl, row, left_pad, "%s",
            ent_path.filename().string().c_str());

  const std::string sz_buf = mng_core::get_ent_sz(ent_path, false),
                    sz_buf_fmt = mng_core::get_ent_sz(ent_path, true);
  row++;
  mvwprintw(dtls_pnl, row, left_pad, "%s",
            ("Size " + sz_buf +
             ((sz_buf != sz_buf_fmt) ? " (" + sz_buf_fmt + ")" : ""))
                .c_str());
  row += 2;
  mvwprintw(dtls_pnl, row, left_pad, "Last accessed: %s",
            mng_core::get_ent_acs_ts(ent_path, "%d-%m-%Y %H:%M:%S").c_str());
  row++;
  mvwprintw(dtls_pnl, row, left_pad, "Last modified: %s",
            mng_core::get_ent_mod_ts(ent_path, "%d-%m-%Y %H:%M:%S").c_str());
  row++;
  mvwprintw(dtls_pnl, row, left_pad, "Last changed: %s",
            mng_core::get_ent_chg_ts(ent_path, "%d-%m-%Y %H:%M:%S").c_str());

  row += 2;
  mvwprintw(dtls_pnl, row, left_pad, "Owner: %s",
            mng_core::get_ent_own(ent_path).c_str());
  row++;
  mvwprintw(dtls_pnl, row, left_pad, "Group: %s",
            mng_core::get_ent_grp(ent_path).c_str());

  row += 2;
  mvwprintw(dtls_pnl, row, left_pad, "Owner permissions: %s",
            mng_core::get_ent_own_perms(ent_path).c_str());
  row++;
  mvwprintw(dtls_pnl, row, left_pad, "Group permissions: %s",
            mng_core::get_ent_grp_perms(ent_path).c_str());
  row++;
  mvwprintw(dtls_pnl, row, left_pad, "Other permissions: %s",
            mng_core::get_ent_oth_perms(ent_path).c_str());
}
