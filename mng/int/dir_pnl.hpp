#pragma once
#include "pnl.hpp"

namespace mng {
class DirPnl : public Pnl {
private:
  char *dir_path;

public:
  DirPnl(unsigned int x, unsigned int y, unsigned int wd, unsigned int ht);
  void prt() override;
  void set_dir_path(const char *dir_path);
};
} // namespace mng
