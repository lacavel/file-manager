#include "mng_gfx/gfx.hpp"
#include "mng_gfx/pnl.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

int main() {
  try {
    mng_gfx::Session gfx;

    mng_gfx::Pnl pnl(0, 0, mng_gfx::get_wd(), mng_gfx::get_ht());

    pnl.set_title("mng_gfx");
    pnl.set_footer("Canvas test");

    auto canvas = pnl.prt();

    canvas.prt(0, 0, "Normal text");

    canvas.prt(0, 2, "Bold text", mng_gfx::Attr::Bold);

    canvas.prt(0, 3, "Dim text", mng_gfx::Attr::Dim);

    canvas.prt(0, 4, "Selected entry", mng_gfx::Attr::Reverse);

    canvas.prt(0, 5, "Underlined text", mng_gfx::Attr::Underline);

    canvas.prt(0, 7, "Unicode: café, 文件");

    mng_gfx::rfsh();

    std::this_thread::sleep_for(std::chrono::seconds(5));

  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
    return 1;
  }

  return 0;
}
