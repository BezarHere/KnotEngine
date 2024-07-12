#pragma once
#include "Base.hpp"

namespace kt
{
  struct Color
  {
    Color() = default;

    Color(real_t rr, real_t gg, real_t bb)
      : r{ rr }, g{ gg }, b{ bb } {
    }

    Color(real_t rr, real_t gg, real_t bb, real_t aa)
      : r{ rr }, g{ gg }, b{ bb }, a{ aa } {
    }

    real_t r = 1;
    real_t g = 1;
    real_t b = 1;
    real_t a = 1;
  };
}
