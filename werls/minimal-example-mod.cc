// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */

  


  // canvas->Fill(0, 0, 255);

  // int center_x = canvas->width() / 2;
  // int center_y = canvas->height() / 2;
  // float radius_max = canvas->width() / 2;
  // float angle_step = 1.0 / 360;
  // for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
  //   if (interrupt_received)
  //     return;
  //   float dot_x = cos(a * 2 * M_PI) * r;
  //   float dot_y = sin(a * 2 * M_PI) * r;
  //   canvas->SetPixel(center_x + dot_x, center_y + dot_y,
  //                    255, 0, 0);
  //   usleep(1 * 1000);  // wait a little to slow down things.
  // }
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  DrawOnCanvas(canvas);    // Using the canvas.

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}

// Simple class that generates a rotating block on the screen.
// class RotatingBlockGenerator : public DemoRunner {
// public:
//   RotatingBlockGenerator(Canvas *m) : DemoRunner(m) {}

//   uint8_t scale_col(int val, int lo, int hi) {
//     if (val < lo) return 0;
//     if (val > hi) return 255;
//     return 255 * (val - lo) / (hi - lo);
//   }

//   void Run() override {
//     const int cent_x = canvas()->width() / 2;
//     const int cent_y = canvas()->height() / 2;

//     // The square to rotate (inner square + black frame) needs to cover the
//     // whole area, even if diagonal. Thus, when rotating, the outer pixels from
//     // the previous frame are cleared.
//     const int rotate_square = min(canvas()->width(), canvas()->height()) * 1.41;
//     const int min_rotate = cent_x - rotate_square / 2;
//     const int max_rotate = cent_x + rotate_square / 2;

//     // The square to display is within the visible area.
//     const int display_square = min(canvas()->width(), canvas()->height()) * 0.7;
//     const int min_display = cent_x - display_square / 2;
//     const int max_display = cent_x + display_square / 2;

//     const float deg_to_rad = 2 * 3.14159265 / 360;
//     int rotation = 0;
//     while (!interrupt_received) {
//       ++rotation;
//       usleep(15 * 1000);
//       rotation %= 360;
//       for (int x = min_rotate; x < max_rotate; ++x) {
//         for (int y = min_rotate; y < max_rotate; ++y) {
//           float rot_x, rot_y;
//           Rotate(x - cent_x, y - cent_x,
//                  deg_to_rad * rotation, &rot_x, &rot_y);
//           if (x >= min_display && x < max_display &&
//               y >= min_display && y < max_display) { // within display square
//             canvas()->SetPixel(rot_x + cent_x, rot_y + cent_y,
//                                scale_col(x, min_display, max_display),
//                                255 - scale_col(y, min_display, max_display),
//                                scale_col(y, min_display, max_display));
//           } else {
//             // black frame.
//             canvas()->SetPixel(rot_x + cent_x, rot_y + cent_y, 0, 0, 0);
//           }
//         }
//       }
//     }
//   }

// private:
//   void Rotate(int x, int y, float angle,
//               float *new_x, float *new_y) {
//     *new_x = x * cosf(angle) - y * sinf(angle);
//     *new_y = x * sinf(angle) + y * cosf(angle);
//   }
// };
