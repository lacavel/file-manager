#pragma once
#include "mng/pnl.hpp"
#include "mng_core/dir_qry.hpp"

#include <filesystem>

namespace mng {
class DirPnl : public Pnl {
private:
  std::filesystem::path dir_path;
  std::vector<mng_core::Ent> dir_buf;
  unsigned int cur_idx = 0;

public:
  DirPnl(std::filesystem::path dir_path);
  void prt() override;
  void set_dir_path(std::filesystem::path dir_path);
  std::filesystem::path get_curr_path() const;
  void hndl_in(int key);
};
} // namespace mng
