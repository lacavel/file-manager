#pragma once
#include "mng/pnl.hpp"

#include <filesystem>

namespace mng {
class DtlsPnl : public Pnl {
private:
  std::filesystem::path ent_path;

public:
  explicit DtlsPnl(std::filesystem::path ent_path);
  void prt() override;
  void set_ent(const std::filesystem::path &ent_path) {
    this->ent_path = ent_path;
  };
};
} // namespace mng
