#include "ctrl.hpp"

#include "mng_core/dir_qry.hpp"
#include "mng_core/ent_qry.hpp"
#include "mng_gfx/gfx.hpp"

#include <algorithm>
#include <utility>

namespace mng {

using namespace mng_core;

static Ent *get_curr_ent(State &st) {
  if (!st.cur_idx || *st.cur_idx >= st.dir_buf.size())
    return nullptr;

  return &st.dir_buf[*st.cur_idx];
}

void qry_curr_ent(State &st) {
  auto *ent = get_curr_ent(st);

  if (!ent || ent->st)
    return;

  auto info = qry_ent(st.dir_path / ent->nm, ent->st_err);

  if (!ent->st_err) {
    ent->type = info.type;
    ent->st = std::move(info);
  }
}

void set_dir(State &st, const std::filesystem::path &path) {
  auto dir_path = std::filesystem::canonical(path);

  auto dir_buf =
      qry_dir(dir_path, {.vis = DirVis::AlmostAll, .mode = QryMode::Basic});

  std::sort(dir_buf.begin(), dir_buf.end(), [](const Ent &a, const Ent &b) {
    const bool a_dir = a.type == EntType::directory;

    const bool b_dir = b.type == EntType::directory;

    if (a_dir != b_dir)
      return a_dir;

    return a.nm < b.nm;
  });

  st.dir_path = std::move(dir_path);
  st.dir_buf = std::move(dir_buf);

  if (st.dir_buf.empty())
    st.cur_idx.reset();
  else
    st.cur_idx = 0;

  if (st.show_dtls)
    qry_curr_ent(st);
}

void rfsh_dir(State &st) {
  const auto *ent = get_curr_ent(st);

  const auto nm = ent ? ent->nm : std::filesystem::path{};

  set_dir(st, st.dir_path);

  for (std::size_t i = 0; i < st.dir_buf.size(); ++i) {
    if (st.dir_buf[i].nm == nm) {
      st.cur_idx = i;
      break;
    }
  }

  if (st.show_dtls)
    qry_curr_ent(st);
}

static void sel_down(State &st, std::size_t col_ht) {
  if (!st.cur_idx || !col_ht)
    return;

  const auto idx = *st.cur_idx;

  if (idx + 1 < st.dir_buf.size() && idx % col_ht < col_ht - 1)
    ++*st.cur_idx;
}

static void sel_up(State &st, std::size_t col_ht) {
  if (!st.cur_idx || !col_ht)
    return;

  if (*st.cur_idx % col_ht > 0)
    --*st.cur_idx;
}

static void sel_left(State &st, std::size_t col_ht) {
  if (!st.cur_idx || !col_ht)
    return;

  if (*st.cur_idx >= col_ht)
    *st.cur_idx -= col_ht;
}

static void sel_right(State &st, std::size_t col_ht) {
  if (!st.cur_idx || !col_ht)
    return;

  if (*st.cur_idx + col_ht < st.dir_buf.size())
    *st.cur_idx += col_ht;
}

static void enter_dir(State &st) {
  const auto *ent = get_curr_ent(st);

  if (!ent)
    return;

  const auto path = st.dir_path / ent->nm;

  if (std::filesystem::is_directory(path))
    set_dir(st, path);
}

static void leave_dir(State &st) {
  const auto prev_path = st.dir_path;
  const auto parent = prev_path.parent_path();

  if (parent.empty() || parent == prev_path)
    return;

  set_dir(st, parent);

  for (std::size_t i = 0; i < st.dir_buf.size(); ++i) {
    if (st.dir_buf[i].nm == prev_path.filename()) {
      st.cur_idx = i;
      break;
    }
  }
}

void hndl_in(State &st, int key, std::size_t col_ht) {
  using mng_gfx::Key;

  st.err.clear();

  try {
    switch (key) {
    case 'q':
      st.running = false;
      return;

    case 'j':
    case static_cast<int>(Key::Down):
      sel_down(st, col_ht);
      break;

    case 'k':
    case static_cast<int>(Key::Up):
      sel_up(st, col_ht);
      break;

    case 'h':
    case static_cast<int>(Key::Left):
      sel_left(st, col_ht);
      break;

    case 'l':
    case static_cast<int>(Key::Right):
      sel_right(st, col_ht);
      break;

    case 'J':
    case static_cast<int>(Key::Enter):
      enter_dir(st);
      break;

    case 'K':
    case static_cast<int>(Key::Backspace):
      leave_dir(st);
      break;

    case 'i':
      st.show_dtls = !st.show_dtls;
      break;

    case 'r':
      rfsh_dir(st);
      break;

    default:
      break;
    }

    if (st.show_dtls)
      qry_curr_ent(st);

  } catch (const std::system_error &err) {
    st.err = err.what();
  }
}
} // namespace mng
