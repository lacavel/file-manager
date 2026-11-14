#pragma once
#include "st.h"
#include <ncurses.h>

typedef struct {
  WINDOW *dir_pnl;
  WINDOW *sts_pnl;
  WINDOW *ver_pnl;
  unsigned int col_wd;
  unsigned int max_rows;
  unsigned int max_cols;
} Ui;

void v_init(Ui *ui, const AppState *st);
void v_sync(Ui *ui, const AppState *st);
void v_draw(const Ui *ui, const AppState *st);
void v_destroy(Ui *ui);
void draw_sts_pnl(const Ui *ui, const AppState *st);
