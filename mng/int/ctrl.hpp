#pragma once
#include "dir_pnl.hpp"
#include <memory>

namespace mng {
class Ctrl {
private:
  std::unique_ptr<DirPnl> dir_pnl;
  bool running = true;

public:
  Ctrl();
  void run();

private:
  void handle_in(int key);
  void upd_pnl();
  void render();
};
} // namespace mng
