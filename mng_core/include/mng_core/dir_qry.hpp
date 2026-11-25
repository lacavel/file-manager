#pragma once

#include "mng_core/ent.hpp"

#include <filesystem>
#include <vector>

namespace mng_core {

enum class DirVis { Normal, AlmostAll, All };

enum class QryMode { Basic, Full };

struct DirQryOpts {
  DirVis vis = DirVis::Normal;
  QryMode mode = QryMode::Basic;
};

std::vector<Ent> qry_dir(const std::filesystem::path &dir_path,
                         const DirQryOpts &opts = {});

} // namespace mng_core
