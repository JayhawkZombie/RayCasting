#pragma once

#include "Vec.hpp"

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
class LightObject
{

public:
  LightObject() = default;
  LightObject(const LightObject &obj)
  {
    Vertices[0] = obj.Vertices[0]; Vertices[1] = obj.Vertices[1]; Vertices[2] = obj.Vertices[2]; Vertices[3] = obj.Vertices[3];
    BlockingShape = obj.BlockingShape;
    Outlines[0] = obj.Outlines[0]; Outlines[1] = obj.Outlines[1]; Outlines[2] = obj.Outlines[2]; Outlines[3] = obj.Outlines[3];
    Points[0] = obj.Points[0]; Points[1] = obj.Points[1]; Points[2] = obj.Points[2]; Points[3] = obj.Points[3];
  }
  ~LightObject() = default;

  void Render(sf::RenderTarget &tgt) {
    tgt.draw(BlockingShape);
    tgt.draw(Outlines[0]);
    tgt.draw(Outlines[1]);
    tgt.draw(Outlines[2]);
    tgt.draw(Outlines[3]);

    tgt.draw(Points[0]);
    tgt.draw(Points[1]);
    tgt.draw(Points[2]);
    tgt.draw(Points[3]);
  }

  void SetPosSize(const sf::Vector2f &pos, const sf::Vector2f &size) {

    BlockingShape.setPosition({ pos.x, pos.y });
    BlockingShape.setSize({ size.x, size.y });
  }

  void SetOutlines() {
    Outlines[0][0].position = sf::Vector2f(Vertices[0].x, Vertices[0].y); Outlines[0][0].color = sf::Color(255, 250, 0);
    Outlines[0][1].position = sf::Vector2f(Vertices[1].x, Vertices[1].y); Outlines[0][1].color = sf::Color(255, 250, 0);

    Outlines[1][0].position = sf::Vector2f(Vertices[1].x, Vertices[1].y); Outlines[1][0].color = sf::Color(255, 255, 255);
    Outlines[1][1].position = sf::Vector2f(Vertices[2].x, Vertices[2].y); Outlines[1][1].color = sf::Color(255, 255, 255);

    Outlines[2][0].position = sf::Vector2f(Vertices[2].x, Vertices[2].y); Outlines[2][0].color = sf::Color(0, 135, 181);
    Outlines[2][1].position = sf::Vector2f(Vertices[3].x, Vertices[3].y); Outlines[2][1].color = sf::Color(0, 135, 181);

    Outlines[3][0].position = sf::Vector2f(Vertices[3].x, Vertices[3].y); Outlines[3][0].color = sf::Color(0, 181, 30);
    Outlines[3][1].position = sf::Vector2f(Vertices[0].x, Vertices[0].y); Outlines[3][1].color = sf::Color(0, 181, 30);
  }

  sf::Vector2f Vertices[4];

  sf::RectangleShape BlockingShape;

  sf::VertexArray Outlines[4]{ sf::VertexArray(sf::Lines, 2), sf::VertexArray(sf::Lines, 2), sf::VertexArray(sf::Lines, 2), sf::VertexArray(sf::Lines, 2) };

  sf::CircleShape Points[4];

  //Line2D<float> Lines[4];

};