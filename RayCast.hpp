#include "Mat.hpp"

#include <iostream>

sf::Vector2f CastRay(const sf::Vector2f &R0, const sf::Vector2f &RA, const sf::Vector2f &B, const sf::Vector2f &T) {

  /**
  *  RA(x2, y2)   * T (x4, y4)        L1: x1 + t * (x2 - x1)
  *      *        |                       y1 + t * (y2 - y1)
  *         *     | L2
  *            *  |                   L2: x3 + v * (x4 - x3)
  *              (*)                      y3 + v * (y4 - y3)
  *               |  * L1
  *               |     *
  *               |        *
  *               B (x3, y3)  * R0 (x1, y1)
  *
  *  Simultaneous Equations:
  *       x1 + t * (x2 - x1) = x3 + v * (x4 - x3)
  *       y1 + t * (y2 - y1) = y3 + v * (y4 - y3)
  *
  *  Into matrix ==>  [ (x2 - x1)   -(x4 - x3)   |   (x3 - x1) ]  ==>  [ X21  -X43 | X31 ]
  *                   [ (y2 - y1)   -(y4 - y3)   |   (y3 - y1) ]       [ Y21  -Y43 | Y31 ]
  */

  float X21 = RA.x - R0.x;
  float X43 = T.x - B.x;
  float X31 = B.x - R0.x;

  float Y21 = RA.y - R0.y;
  float Y43 = T.y - B.y;
  float Y31 = B.y - R0.y;

  Mat2D system(X21, -X43, Y21, -Y43, X31, Y31);
  float t = 0, s = 0;

  try
  {
    system.SolveSystem();
    t = system.e;
    s = system.f;
  } catch (std::runtime_error &e)
  {
    std::cerr << "Matrix error: " << e.what() << std::endl;
  }

  float x = R0.x + std::abs(t) * X21;
  float y = R0.y + std::abs(t) * Y21;

  return sf::Vector2f(x, y);
}