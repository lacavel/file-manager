#pragma once

#include "mng_core/ent_stat.hpp"

#include <string>
#include <string_view>

namespace mng {

std::string fmt_sz(std::uintmax_t sz);

std::string fmt_ts(mng_core::EntTs ts);

std::string fmt_perms(mng_core::EntPerms perms);

std::string_view fmt_type(mng_core::EntType type);

std::string_view get_ent_icon(mng_core::EntType type);

} // namespace mng
