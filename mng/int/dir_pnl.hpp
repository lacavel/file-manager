#pragma once
#include "mng_core/dir_qry.hpp"
#include "pnl.hpp"

#include <filesystem>

namespace mng {
class DirPnl : public Pnl {
private:
  std::filesystem::path dir_path;
  std::vector<mng_core::Ent> dir_buf;
  unsigned int cur_idx = 0;

public:
  DirPnl(unsigned int x, unsigned int y, unsigned int wd, unsigned int ht);
  void prt() override;
  void set_dir(const std::filesystem::path &dir_path);
  void hndl_in(int key);
};
} // namespace mng
