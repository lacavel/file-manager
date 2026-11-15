#include "mng_core/e_utils.h"

#include "cntlr.h"
#include "v.h"
#include <limits.h>

int main(int argc, char **argv) {
  AppState st = {0};
  get_abs_path(st.path, (argc == 2 && argv[1]) ? argv[1] : ".");

  st.dir_buf = qry_dir(st.path);
  if (!st.dir_buf)
    return 1;
  while (st.dir_buf[st.ent_cnt]->ent_nm[0] != '\0')
    st.ent_cnt++;

  Ui ui;
  v_init(&ui, &st);
  v_draw(&ui, &st);

  int ch;
  bool running = true;
  while (running && (ch = wgetch(ui.dir_pnl)) != ERR) {
    running = cntlr_handle(&st, ch, &ui);
    v_sync(&ui, &st);
    v_draw(&ui, &st);
  }

  v_destroy(&ui);
  free_dir(st.dir_buf, st.ent_cnt);
  return 0;
}
