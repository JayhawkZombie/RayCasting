#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "LightSystem.h"
#include "LightObject.h"
#include "Mat.hpp"


struct light_source {
  sf::Vector2f pos;
  float attenuation;
};

std::shared_ptr<sf::RenderTexture> LightTexture;

void RecreateLightTexture(sf::RenderTexture &LightTexture, sf::Shader &LightFragShader, const sf::Color &color, const float &radius, sf::CircleShape &circle)
{
  LightFragShader.setParameter("color", color);
  LightFragShader.setParameter("center", { 400.f, 400.f });
  LightFragShader.setParameter("radius", { 800.f });
  LightFragShader.setParameter("expand", 0.001f);

  circle.setRadius(radius);
  circle.setFillColor(sf::Color::Transparent);

  LightTexture.clear(sf::Color::Transparent);

  LightTexture.draw(circle, &LightFragShader);
  LightTexture.display();
}

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
  window.setVerticalSyncEnabled(false);

  sf::Texture tex;
  if (!tex.loadFromFile("SFENGINELOGO.png")) {
    return EXIT_FAILURE;
  }

  sf::Shader LightFragShader;
  if (!LightFragShader.loadFromFile("Lighting_Frag.fsh", sf::Shader::Fragment)) {
    return EXIT_FAILURE;
  }

  LightSystem system;
  system.window_size_x = window.getSize().x;
  system.window_size_y = window.getSize().y;
  system.OverallBounds.setPosition({ 0, 0 });
  system.OverallBounds.setSize({ (float)window.getSize().x, (float)window.getSize().y });
  system.OverallBounds.setFillColor(sf::Color(102, 102, 102));
  system.OverallBounds.setTexture(&tex);

  system.AddLightObject({ 60, 70 }, { 180, 180 }, sf::Color(38, 38, 38));
  system.AddLightObject({ 70, 150 }, { 100, 100 }, sf::Color(38, 38, 38));
  system.AddLightObject({ 50, 600 }, { 125, 35 }, sf::Color(38, 38, 38));
  system.AddLightObject({ 550, 60 }, { 200, 90 }, sf::Color(38, 38, 38));
  system.AddLightObject({ 200, 200 }, { 80, 180 }, sf::Color(38, 38, 38));
  system.AddLightObject({ 500, 550 }, { 80, 40 }, sf::Color(38, 38, 38));
  system.AddLightObject({ 700, 500 }, { 80, 40 }, sf::Color(38, 38, 38));

  system.AddLight({ 400, 400 }, 400, sf::Color::Blue);
  //system.ComputeVertexArray();

  std::size_t tri_to_draw = 0;
  window.setKeyRepeatEnabled(false);

  sf::CircleShape circle;
  circle.setRadius(800.f);
  circle.setOrigin(circle.getRadius(), circle.getRadius());
  circle.setPosition(sf::Vector2f(window.getSize()) / 2.f);
  circle.setFillColor(sf::Color::Transparent);

  sf::RenderTexture LightTexture;
  LightTexture.create(800, 800);

  std::size_t current_color = 0;
  std::vector<sf::Color> Colors;
  Colors.push_back(sf::Color(204, 0, 232, 160));
  Colors.push_back(sf::Color(0, 216, 232, 160));
  Colors.push_back(sf::Color(255, 255, 255, 160));
  Colors.push_back(sf::Color(0, 33, 255, 160));

  float CircleRadius = 800.f;
  float Attenuation = 800.f;

  LightFragShader.setParameter("color", Colors[3]);
  LightFragShader.setParameter("center", { 400.f, 400.f });
  LightFragShader.setParameter("radius", { CircleRadius });
  LightFragShader.setParameter("expand", 0.001f);
  LightFragShader.setParameter("windowHeight", static_cast<float>(window.getSize().y));

  LightTexture.draw(circle, &LightFragShader);
  LightTexture.display();

  sf::RenderStates LightSystemState;
  LightSystemState.blendMode = sf::BlendAlpha;
  LightSystemState.texture = &LightTexture.getTexture();

  system.LightTexture = &LightTexture;

  srand(NULL);

  bool DrawShaderCircle = true;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Space) {
          system.theta = 0;
          system.AdvanceSweep({ 400.f, 400.f }, 400.f);
        }
        else if (event.key.code == sf::Keyboard::Up)
          system.frame_delta += 1;
        else if (event.key.code == sf::Keyboard::Down)
          system.frame_delta -= 1;
        else if (event.key.code == sf::Keyboard::R) {
          CircleRadius += 10;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          LightSystemState.texture = &LightTexture.getTexture();
          LightSystemState.texture = &LightTexture.getTexture();
        }
        else if (event.key.code == sf::Keyboard::A) {
          CircleRadius -= 10;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          LightSystemState.texture = &LightTexture.getTexture();
          LightSystemState.texture = &LightTexture.getTexture();
        }
        else if (event.key.code == sf::Keyboard::Right)
          system.breaks_around_circle += 10;
        else if (event.key.code == sf::Keyboard::Left)
          system.breaks_around_circle -= 10;
        else if (event.key.code == sf::Keyboard::C) {
          //change colors, so redraw the light texture and assign to the light system
          current_color = (current_color + 1) % Colors.size();
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          LightSystemState.texture = &LightTexture.getTexture();
          LightSystemState.texture = &LightTexture.getTexture();
        } //c clicked, change colors
      }
		}

    LightSystemState.blendMode = sf::BlendAlpha;

		window.clear();

    system.AdvanceSweep(sf::Vector2f(sf::Mouse::getPosition(window)), CircleRadius);

    system.Render(window, LightSystemState);

		window.display();
	}

	return 0;
}