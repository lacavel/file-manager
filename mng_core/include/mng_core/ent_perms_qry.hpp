#pragma once
#include <filesystem>
#include <string>

namespace mng_core {
std::string get_ent_own_perms(const std::filesystem::path &ent_path);
std::string get_ent_grp_perms(const std::filesystem::path &ent_path);
std::string get_ent_oth_perms(const std::filesystem::path &ent_path);
std::string get_ent_own(const std::filesystem::path &ent_path);
std::string get_ent_grp(const std::filesystem::path &ent_path);
} // namespace mng_core
