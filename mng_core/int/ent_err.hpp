#pragma once
#include <cerrno>
#include <filesystem>
#include <string>
#include <system_error>

[[noreturn]] inline void ent_err(const char *op,
                                 const std::filesystem::path &p) {
  throw std::system_error(errno, std::generic_category(),
                          std::string(op) + ": " + p.string());
}
