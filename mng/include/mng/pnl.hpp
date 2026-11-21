#pragma once
#include <memory>

namespace mng {

struct PnlImpl;

class Pnl {
private:
  std::unique_ptr<PnlImpl> pimpl;

protected:
  void *get_hndl() const;

public:
  Pnl(unsigned int x, unsigned int y, unsigned int wd, unsigned int ht);
  virtual ~Pnl();

  Pnl(Pnl &&oth) noexcept;
  Pnl &operator=(Pnl &&oth) noexcept;
  Pnl(const Pnl &) = delete;
  Pnl &operator=(const Pnl &) = delete;

  void resize(unsigned int x, unsigned int y, unsigned int wd, unsigned int ht);
  void set_vis(bool vis);
  bool get_vis();
  static void rfsh();

  virtual void prt() = 0;
};
} // namespace mng
