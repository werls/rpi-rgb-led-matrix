// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <chrono>
#include <algorithm>

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {

  auto start = std::chrono::steady_clock::now();

  while (!interrupt_received) {
  for (int y = 0; y < canvas->height(); y++) {
    for (int x = 0; x < canvas->width(); x++) {

      auto now = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
      
      int r = (1 + sin(x + elapsed) / 8.0) * 127;
      int g = (1 + sin(y + elapsed) / 8.0) * 127;
      int b = (1 + sin(((x + y) + elapsed) / 8.0)) * 127;

      r = std::clamp(r, 0, 255);
      g = std::clamp(g, 0, 255);
      b = std::clamp(b, 0, 255);

      canvas->SetPixel(x, y, r, g, b);
    }
  }
  }

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