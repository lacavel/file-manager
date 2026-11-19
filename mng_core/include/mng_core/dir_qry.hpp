#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace mng_core {

struct Ent {
  std::string ent_type;
  std::string ent_nm;
};

std::vector<Ent> qry_dir(const std::filesystem::path &dir_path);
} // namespace mng_core
