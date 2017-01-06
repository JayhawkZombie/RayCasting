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
  LightFragShader.setParameter("radius", { radius });
  LightFragShader.setParameter("expand", 0.2f);

  circle.setRadius(radius);
  circle.setOrigin(circle.getRadius(), circle.getRadius());
  circle.setPosition(sf::Vector2f({ 400.f, 400.f }));
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

  system.AddLightObject({ 60, 70 }, { 180, 180 }, sf::Color::Black);
  system.AddLightObject({ 70, 150 }, { 100, 100 }, sf::Color::Black);
  system.AddLightObject({ 50, 600 }, { 125, 35 }, sf::Color::Black);
  system.AddLightObject({ 550, 60 }, { 200, 90 }, sf::Color::Black);
  system.AddLightObject({ 200, 200 }, { 80, 180 }, sf::Color::Black);
  system.AddLightObject({ 500, 550 }, { 80, 40 }, sf::Color::Black);
  system.AddLightObject({ 700, 500 }, { 80, 40 }, sf::Color::Black);
  system.AddLightObject({ 300, 650 }, { 30, 25 }, sf::Color::Black);

  system.AddLight({ 400, 400 }, 400, sf::Color::Blue);

  std::size_t tri_to_draw = 0;
  window.setKeyRepeatEnabled(true);

  float CircleRadius = 400.f;

  sf::CircleShape circle;
  circle.setRadius(CircleRadius);
  circle.setOrigin(circle.getRadius(), circle.getRadius());
  circle.setPosition(sf::Vector2f(window.getSize()) / 2.f);
  circle.setFillColor(sf::Color::Transparent);

  sf::RenderTexture LightTexture;
  LightTexture.create(800, 800);

  std::size_t current_color = 0;
  std::vector<sf::Color> Colors;
  Colors.push_back(sf::Color(204, 0, 232, 100));
  Colors.push_back(sf::Color(0, 216, 232, 100));
  Colors.push_back(sf::Color(255, 255, 255, 100));
  Colors.push_back(sf::Color(0, 33, 255, 100));

  
  //float Attenuation = 800.f;
  sf::Uint8 Intensity = 100;

  LightFragShader.setParameter("color", Colors[3]);
  LightFragShader.setParameter("center", { 400.f, 400.f });
  LightFragShader.setParameter("radius", { CircleRadius });
  LightFragShader.setParameter("expand", 0.2f);
  LightFragShader.setParameter("windowHeight", static_cast<float>(window.getSize().y));

  LightTexture.draw(circle, &LightFragShader);
  LightTexture.display();

  sf::RenderStates LightSystemState;
  LightSystemState.blendMode = sf::BlendMultiply;
  LightSystemState.texture = &LightTexture.getTexture();

  system.LightTexture = &LightTexture;

  bool DrawShaderCircle = true;
  bool paused = false;

  const std::size_t steps = 1000;

  sf::Vector2f LightSpots[50] = 
  {

  };

  sf::Vector2f Light1Center(100, 600);
  sf::Vector2f Light2Center(450, 150);

  sf::Vector2f Light1Pos(200, 400);
  sf::Vector2f Light2Pos(600, 600);

  float Light1Theta = 0;
  float Light2Theta = 0;

  float Light1PathRad = 100;
  float Light2PathRad = 50;

  bool PrintPos = false;

  system.AddComplexObject({ 
    sf::Vector2f(400, 300) , 
    sf::Vector2f(340, 380), 
    sf::Vector2f(340, 340), 
    sf::Vector2f(340, 320),
    sf::Vector2f(400, 400)
  
  });

  system.AddComplexObject({
    sf::Vector2f(500, 500) ,
    sf::Vector2f(340, 580),
    sf::Vector2f(340, 540),
    sf::Vector2f(440, 520),
    sf::Vector2f(400, 300)

  });

  bool secondlight = false;
  bool thirdlight = false;

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

        else if (event.key.code == sf::Keyboard::Up) {
          CircleRadius += 10;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          //LightSystemState.texture = &LightTexture.getTexture();
        }

        else if (event.key.code == sf::Keyboard::Down) {
          CircleRadius -= 10;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          //LightSystemState.texture = &LightTexture.getTexture();
        }

        else if (event.key.code == sf::Keyboard::Q) {
          Intensity += 5;
          for (auto & c : Colors)
            c.a = Intensity;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
        }
        else if (event.key.code == sf::Keyboard::A) {
          Intensity -= 5;
          for (auto & c : Colors)
            c.a = Intensity;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
        }

        else if (event.key.code == sf::Keyboard::Right) {
          system.breaks_around_circle = std::min(1500, system.breaks_around_circle + 50);
        }

        else if (event.key.code == sf::Keyboard::Left)
          system.breaks_around_circle = std::max(200, system.breaks_around_circle - 50);

        else if (event.key.code == sf::Keyboard::C) {
          //change colors, so redraw the light texture and assign to the light system
          current_color = (current_color + 1) % Colors.size();
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          //LightSystemState.texture = &LightTexture.getTexture();
        } //c clicked, change colors
        else if (event.key.code == sf::Keyboard::P) {
          paused = !paused;
          system.StatusText.setString(paused ? "Paused" : "Playing");
          system.StatusText.setFillColor(paused ? sf::Color::Red : sf::Color::Green);
        }
        else if (event.key.code == sf::Keyboard::Num2) {
          secondlight = !secondlight;
          system.SecondLightText.setString(secondlight ? "Second Light: Enabled" : "Second Light: Disabled");
          //Light1Theta = 0;
        }
        else if (event.key.code == sf::Keyboard::Num3) {
          thirdlight = !thirdlight;
          system.ThirdLightText.setString(thirdlight ? "Third Light: Enabled" : "Third Light: Disabled");
          //Light2Theta = 0;
        }
      }
		}

    LightSystemState.blendMode = sf::BlendAdd;

		window.clear();

    if (!paused) {
      Light1Theta += 2 * PI / 1000;
      Light2Theta += 2 * PI / 1000;

      sf::Vector2f Light1Dir(std::cos(Light1Theta), std::sin(Light1Theta));
      sf::Vector2f Light2Dir(std::cos(Light2Theta), std::sin(Light2Theta));

      Light1Pos = Light1Center + Light1Dir * Light1PathRad;
      Light2Pos = Light2Center + Light2Dir * Light2PathRad;


      system.RefreshFrame();
      system.AdvanceSweep(sf::Vector2f(sf::Mouse::getPosition(window)), CircleRadius);
      if (secondlight)
        system.AdvanceSweep(Light1Pos, 800.f);
      if (thirdlight)
        system.AdvanceSweep(Light2Pos, 800.f);
    }

    system.Render(window, LightSystemState);

		window.display();
	}

	return 0;
}