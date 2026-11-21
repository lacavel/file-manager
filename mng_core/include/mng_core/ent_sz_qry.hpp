#pragma once
#include <filesystem>
#include <string>

namespace mng_core {
std::string get_ent_sz(const std::filesystem::path &ent_path, bool fmt = true);
}
