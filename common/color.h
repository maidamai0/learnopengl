#pragma once

#include <cassert>
#include <string>

#define COLOR(c) c.R, c.G, c.B

namespace color {
struct Color {
  Color(const int r, const int g, const int b, const int a = 255)
      : R{static_cast<float>(r) / 255.0F},
        G{static_cast<float>(g) / 255.0F},
        B{static_cast<float>(b) / 255.0F},
        A{static_cast<float>(a) / 255.0F} {
  }

  explicit Color(std::string&& color) {
    assert(color[0] == '#');
    int num = stoi(color.substr(1), nullptr, 16);
    R = static_cast<float>(num / 0x10000) / 255.F;
    G = static_cast<float>((num / 0x100) % 0x100) / 255.F;
    B = static_cast<float>(num % 0x100) / 255.F;
  }

  float R = 0.0F;
  float G = 0.0F;
  float B = 0.0F;
  float A = 1.0F;
};

const Color white("#ffffff");
const Color linen("#faf0e6");
const Color oldlace("#f5fffa");
const Color seashell("#fff5ee");
const Color floralwhite("#fffaf0");
const Color whitesmoke("#f5f5f5");

const Color black("#000000");

}  // namespace color