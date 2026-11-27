#pragma once

#include "mng_core/ent.hpp"

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace mng {

struct State {
  std::filesystem::path dir_path;

  std::vector<mng_core::Ent> dir_buf;

  std::optional<std::size_t> cur_idx;

  bool show_dtls = false;

  bool running = true;

  std::string err;
};

} // namespace mng
