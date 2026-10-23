#include "mng_core/ent.h"

#include <stdio.h>

int main(int argc, char **argv) {
  Ent *dir = NULL;

  size_t ent_cnt =
      pop_dir(&dir, (argc == 2 && argv[1] != NULL) ? argv[1] : ".");

  for (size_t i = 0; i <= ent_cnt; i++)
    printf("%s %s\n", dir[i].ent_type, dir[i].ent_nm);

  free_dir(dir, ent_cnt);
  return 0;
}
