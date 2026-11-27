#include "v.hpp"
#include "ent_fmt.hpp"

#include "mng_core/id_qry.hpp"
#include "mng_gfx/gfx.hpp"
#include "mng_gfx/tbl.hpp"

#include <format>

namespace mng {

using namespace mng_gfx;
using namespace mng_core;

static TblData mk_dir_data(const State &st) {
  TblData dta;

  dta.lyt = TblLyt::Flow;
  dta.cur_idx = st.cur_idx;

  dta.rows.reserve(st.dir_buf.size());

  for (std::size_t i = 0; i < st.dir_buf.size(); ++i) {
    const auto &ent = st.dir_buf[i];

    const auto attr =
        st.cur_idx && *st.cur_idx == i ? Attr::Reverse : Attr::Normal;

    dta.rows.push_back(
        {{std::format("{}{}", get_ent_icon(ent.type), ent.nm.string()), attr}});
  }

  return dta;
}

static std::string fmt_id(const std::optional<EntId> &id, bool usr) {
  if (!id)
    return "?";

  try {
    const auto nm = usr ? qry_usr(*id) : qry_grp(*id);

    if (nm)
      return *nm;

  } catch (const std::system_error &) {
  }

  return std::to_string(*id);
}

static void prt_dtls(Canvas &canvas, const Ent &ent) {
  using P = EntPerms;
  const auto ht = canvas.get_ht();
  if (!ht || !canvas.get_wd())
    return;
  unsigned int y = 0;
  const auto body_ht = ent.st ? (ht >= 2 ? ht - 2 : 0) : ht;
  const auto add = [&](std::string_view txt, Attr attr = Attr::Normal) {
    if (y < body_ht)
      canvas.prt(0, y++, txt, attr);
  };

  const auto sep = [&]() {
    if (y < body_ht)
      ++y;
  };

  add(fmt_type(ent.type), Attr::Bold);

  if (!ent.st) {
    sep();

    add(ent.st_err ? ent.st_err.message() : "Metadata unavailable");

    return;
  }
  const auto &st = *ent.st;
  if (st.sz) {
    if (*st.sz >= 1024)
      add(std::format("Size: {} B ({})", *st.sz, fmt_sz(*st.sz)));
    else
      add(std::format("Size: {} B", *st.sz));
  } else
    add("Size: ?");
  add(std::format("Links: {}", st.lnk_cnt ? std::to_string(*st.lnk_cnt) : "?"));
  sep();
  add("Permissions", Attr::Bold);
  add(std::format("Owner: {}", fmt_id(st.uid, true)));
  add(std::format("Group: {}", fmt_id(st.gid, false)));
  sep();
  add(std::format("Owner: {}", fmt_perm_icons(st.perms, P::owner_read,
                                              P::owner_write, P::owner_exec)));
  add(std::format("Group: {}", fmt_perm_icons(st.perms, P::group_read,
                                              P::group_write, P::group_exec)));
  add(std::format("Other: {}",
                  fmt_perm_icons(st.perms, P::others_read, P::others_write,
                                 P::others_exec)));
  sep();
  add("Timestamps", Attr::Bold);
  add(std::format("Access: {}", st.acs_ts ? fmt_ts(*st.acs_ts) : "?"));
  add(std::format("Modify: {}", st.mod_ts ? fmt_ts(*st.mod_ts) : "?"));
  add(std::format("Change: {}", st.chg_ts ? fmt_ts(*st.chg_ts) : "?"));
  if (st.birth_ts)
    add(std::format("Birth: {}", fmt_ts(*st.birth_ts)));
  if (ht >= 2)
    canvas.prt(
        0, ht - 2,
        std::format("Inode: {}", st.ino ? std::to_string(*st.ino) : "?"));

  canvas.prt(0, ht - 1,
             std::format("Device: {}", st.dev ? std::to_string(*st.dev) : "?"));
}

static void upd_lyt(Pnl &dir_pnl, Pnl &dtls_pnl, bool show_dtls) {
  const auto wd = get_wd();
  const auto ht = get_ht();

  if (!wd || !ht)
    return;

  if (show_dtls) {
    const auto dtls_wd = wd / 3;
    const auto dir_wd = wd - dtls_wd;

    dir_pnl.resize(0, 0, dir_wd, ht);

    dtls_pnl.resize(dir_wd, 0, dtls_wd, ht);

    if (!dtls_pnl.get_vis())
      dtls_pnl.set_vis(true);

  } else {
    if (dtls_pnl.get_vis())
      dtls_pnl.set_vis(false);

    dir_pnl.resize(0, 0, wd, ht);
  }
}

void prt_view(Pnl &dir_pnl, Pnl &dtls_pnl, const State &st) {
  const auto wd = get_wd();
  const auto ht = get_ht();

  if (!wd || !ht)
    return;

  const bool show_dtls = st.show_dtls && wd >= 60 && ht >= 3;

  upd_lyt(dir_pnl, dtls_pnl, show_dtls);

  const Tbl tbl;

  dir_pnl.set_title(st.dir_path.string());

  if (st.err.empty())
    dir_pnl.set_footer(std::format("{} entries", st.dir_buf.size()));
  else
    dir_pnl.set_footer(st.err);

  auto dir_canvas = dir_pnl.prt();

  tbl.prt(dir_canvas, mk_dir_data(st));

  if (show_dtls) {
    if (st.cur_idx && *st.cur_idx < st.dir_buf.size()) {
      const auto &ent = st.dir_buf[*st.cur_idx];

      dtls_pnl.set_title(ent.nm.string());
      auto dtls_canvas = dtls_pnl.prt();
      prt_dtls(dtls_canvas, ent);

    } else {
      dtls_pnl.set_title("Details");

      auto dtls_canvas = dtls_pnl.prt();

      dtls_canvas.prt(0, 0, "No entry selected");
    }
  }

  rfsh();
}

} // namespace mng
