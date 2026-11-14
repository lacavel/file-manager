#include "cntlr.h"
#include <ncurses.h>

static void nav(AppState *st, int key, unsigned int max_rows,
                unsigned int max_cols) {
  if (st->ent_cnt == 0 || max_rows == 0)
    return;

  unsigned int row = st->sel % max_rows;
  unsigned int col = st->sel / max_rows;
  switch (key) {
  case 'k':
  case KEY_UP:
    if (row > 0)
      st->sel--;
    break;
  case 'j':
  case KEY_DOWN:
    if (row < max_rows - 1 && st->sel < st->ent_cnt - 1)
      st->sel++;
    break;
  case 'h':
  case KEY_LEFT:
    if (col > 0) {
      st->sel = (col - 1) * max_rows + row;
      if (st->sel >= st->ent_cnt)
        st->sel = st->ent_cnt - 1;
    }
    break;
  case 'l':
  case KEY_RIGHT:
    if (col < max_cols - 1) {
      size_t new_idx = (col + 1) * max_rows + row;
      if (new_idx < st->ent_cnt)
        st->sel = new_idx;
    }
    break;
  default:
    break;
  }
}

bool cntlr_handle(AppState *st, int key, const Ui *ui) {
  switch (key) {
  case 'q':
    return false;
  case 'i':
    st->sts_visible = !st->sts_visible;
    break;
  case KEY_RESIZE:
    break;
  default:
    nav(st, key, ui->max_rows, ui->max_cols);
    break;
  }
  return true;
}
