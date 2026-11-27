#include "args.h"
#include "ctrl.hpp"
#include "st.hpp"
#include "v.hpp"

#include "mng_gfx/gfx.hpp"
#include "mng_gfx/pnl.hpp"

#include <exception>
#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
  Args args;
  parse_args(argc, argv, &args);

  try {
    mng::State st;

    mng::set_dir(st, args.init_path ? std::filesystem::path(args.init_path)
                                    : std::filesystem::current_path());

    mng_gfx::Session gfx;

    const auto wd = mng_gfx::get_wd();
    const auto ht = mng_gfx::get_ht();

    mng_gfx::Pnl dir_pnl(0, 0, wd, ht);

    mng_gfx::Pnl dtls_pnl(0, 0, 1, 1);

    dtls_pnl.set_vis(false);

    mng::prt_view(dir_pnl, dtls_pnl, st);

    while (st.running) {
      const int key = mng_gfx::get_key();

      mng::hndl_in(st, key);

      if (st.running)
        mng::prt_view(dir_pnl, dtls_pnl, st);
    }

  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
    return 1;
  }

  return 0;
}
