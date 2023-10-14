/* Copyright 2023, Ben Combee.  All rights reserved.

   This project is licensed under the BSD 3-clause license.  See LICENSE for
   details.. */

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/platform.h"

#include "common/pimoroni_common.hpp"
#include "libraries/badger2040/badger2040.hpp"

#include "dvdlogo.h"

using namespace pimoroni;

Badger2040 badger;

constexpr int WIDTH = 296;
constexpr int HEIGHT = 128;

int align_to_8(int n) {
  return ((n + 7) / 8) * 8;
}

class DVDLogo {
  int x = 20, y = 20, dx = 2, dy = 2;
  static constexpr int x_max = WIDTH - dvd_logo_68x30_1bpp_w;
  static constexpr int y_max = HEIGHT - dvd_logo_68x30_1bpp_h;
  bool first_pass = true;
  int frameNo = 0;

  void draw_logo() {
      badger.pen(15);
      badger.clear();
      badger.image(
        dvd_logo_68x30_1bpp,
        dvd_logo_68x30_1bpp_stride,
        0, 0,
        dvd_logo_68x30_1bpp_w, dvd_logo_68x30_1bpp_h,
        x, y);
  }

  void move_logo() {
    x += dx;
    if (x < 0) { x = 0; dx = -dx; }
    if (x > x_max) { x = x_max; dx = -dx; }

    y += dy;
    if (y < 0) { y = 0; dy = -dy; }
    if (y > y_max) { y = y_max; dy = -dy; }

    printf("move_logo to (%d,%d) dx/dy (%d,%d)\n", x, y, dx, dy);
  }

  void update_screen() {
    constexpr int update_margin = 16;

    if (first_pass) {
      badger.update();
      first_pass = false;
    } else {
      int upd_x = std::max(x - update_margin, 0);
      int upd_x2 = std::min(WIDTH, x + dvd_logo_68x30_1bpp_w + update_margin);
      int upd_w = upd_x2 - upd_x;

      int upd_y = align_to_8(std::max(y - update_margin, 0) - 7);
      int upd_y2 = align_to_8(std::min(HEIGHT, y + dvd_logo_68x30_1bpp_h + update_margin));
      int upd_h = upd_y2 - upd_y;

      printf("updating (%d,%d) w/h (%d,%d)\n", upd_x, upd_y, upd_w, upd_y);
      badger.partial_update(upd_x, upd_y, upd_w, upd_h);
    }
  }

public:

  void loop() {
    move_logo();
    draw_logo();
    update_screen();
    sleep_ms(100);
    badger.led(40 * (frameNo % 2));
    frameNo++;
  }

  void init() {
    stdio_init_all();
    sleep_ms(2000);
    puts("DVDLogo Starting\n");
    badger.init();
    badger.update_speed(3);
  }

} dvd_logo;

int main() {
  dvd_logo.init();
  while (true) {
    dvd_logo.loop();
  }
}
