#pragma once

#include "st.hpp"

namespace mng {
void set_dir(State &st, const std::filesystem::path &path);
void rfsh_dir(State &st);
void qry_curr_ent(State &st);
void hndl_in(State &st, int key, std::size_t col_ht);
} // namespace mng
