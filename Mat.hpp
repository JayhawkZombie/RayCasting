#include <math.h>
#include <cmath>
#pragma once
#include <algorithm>
#include <stdexcept>

class Mat2D
{
public:
  Mat2D()
    : a(1), b(1), c(1), d(1), e(1), f(1)
  {}
  Mat2D(const Mat2D &other)
    : a(other.a), b(other.b), c(other.c), d(other.d), e(other.e), f(other.f)
  {}
  Mat2D(const float &_a, const float &_b, const float &_c, const float &_d, const float &_e, const float &_f)
    :
    a(_a), b(_b), c(_c), d(_d), e(_e), f(_f)
  {}

  void SolveSystem() {
    //make sure the det is not zero

    float det = ( a * d ) - ( c * b );

    //if (std::abs(det) < 0.0001)
    //  throw std::runtime_error("Matrix is singular or there is no stable numerical solution");

    /**
    * [  a   b   |   e  ]
    * [  c   d   |   f  ]
    *  Gaussean Elimination (with partial pivoting)
    * if (abs(c) > abs(a)) swap row(1) & row(2)
    */
    if (std::abs(c) > std::abs(a)) {
      std::swap(a, c);
      std::swap(b, d);
      std::swap(e, f);
    }

    /**
    * [  a   b   |   e  ]
    * [  c   d   |   f  ]
    *
    *  r2 = r2 - (c/a) * r1
    */
    float factor = c / a;
    float r2c = factor * a;  c -= r2c;
    float r2d = factor * b;  d -= r2d;
    float r2f = factor * e;  f -= r2f;

    /**
    *  [  a          b          |          e         ] == became ==> [ a b | e ]
    *  [  0   d - (c / a) * b   |   f - (c / a) * e  ]               [ 0 d | f ]
    *
    * v = [ f - (c / d) * e ] / [ d - (c / a) * b ]
    */

    f /= d; d = 1;
    /**
    *  [  a  b  |  e  ]
    *  [  0  1  |  f  ]
    *
    *  b -= b * 1;
    *  e -= b * f; <-- do first
    */
    e -= b * f; b = 1;
    /**
    *  [  a  0  |  e  ]
    *  [  0  1  |  f  ]
    */
    e /= a; a = 1;
    /**
    *  [  1  0  |  e  ]
    *  [  0  1  |  f  ]
    *
    *   t = e
    *   v = f
    */

  }

  float a, b, c, d, e, f;

};