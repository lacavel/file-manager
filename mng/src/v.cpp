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

static TblData mk_dtls_data(const Ent &ent) {
  TblData dta;

  dta.lyt = TblLyt::Grid;
  dta.col_gap = 1;

  const auto add = [&dta](std::string label, std::string val) {
    dta.rows.push_back({{std::move(label), Attr::Bold}, {std::move(val)}});
  };

  const auto sep = [&dta]() { dta.rows.emplace_back(); };

  add("Type:", std::string(fmt_type(ent.type)));

  if (!ent.st) {
    sep();

    dta.rows.push_back(
        {{ent.st_err ? ent.st_err.message() : "Metadata unavailable"}});

    return dta;
  }

  const auto &st = *ent.st;

  add("Size:", st.sz ? fmt_sz(*st.sz) : "?");

  add("Bytes:", st.sz ? std::to_string(*st.sz) : "?");

  add("Links:", st.lnk_cnt ? std::to_string(*st.lnk_cnt) : "?");

  sep();

  add("Owner:", fmt_id(st.uid, true));
  add("Group:", fmt_id(st.gid, false));
  add("Perms:", fmt_perms(st.perms));

  sep();

  add("Access:", st.acs_ts ? fmt_ts(*st.acs_ts) : "?");

  add("Modify:", st.mod_ts ? fmt_ts(*st.mod_ts) : "?");

  add("Change:", st.chg_ts ? fmt_ts(*st.chg_ts) : "?");

  if (st.birth_ts)
    add("Birth:", fmt_ts(*st.birth_ts));

  sep();

  if (st.ino)
    add("Inode:", std::to_string(*st.ino));

  if (st.dev)
    add("Device:", std::to_string(*st.dev));

  return dta;
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

      tbl.prt(dtls_canvas, mk_dtls_data(ent));

    } else {
      dtls_pnl.set_title("Details");

      auto dtls_canvas = dtls_pnl.prt();

      dtls_canvas.prt(0, 0, "No entry selected");
    }
  }

  rfsh();
}

} // namespace mng
