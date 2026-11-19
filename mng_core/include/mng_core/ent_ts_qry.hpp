#pragma once
#include <filesystem>
#include <string>
#include <string_view>

inline constexpr std::string_view def_ts_fmt = "%d-%m-%Y %H:%M:%S";

std::string get_ent_acs_ts(const std::filesystem::path &ent_path,
                           std::string_view ts_fmt = def_ts_fmt);
std::string get_ent_mod_ts(const std::filesystem::path &ent_path,
                           std::string_view ts_fmt = def_ts_fmt);
std::string get_ent_chg_ts(const std::filesystem::path &ent_path,
                           std::string_view ts_fmt = def_ts_fmt);
