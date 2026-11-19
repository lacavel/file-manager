#pragma once
#include <filesystem>
#include <sys/stat.h>

struct stat get_ent_sys(const std::filesystem::path &ent_path, bool flw);
