#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
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
  sf::Glsl::Vec4 circle_color = { static_cast<float>( color.r ), static_cast<float>( color.g ), static_cast<float>( color.b ), static_cast<float>( color.a ) };
  sf::Glsl::Vec2 center = { 400.f, 400.f };

  LightFragShader.setUniform("color", circle_color);
  LightFragShader.setUniform("center", center);
  LightFragShader.setUniform("radius", radius);
  LightFragShader.setUniform("expand", 0.2f);

  circle.setRadius(radius);
  circle.setOrigin(circle.getRadius(), circle.getRadius());
  circle.setPosition(sf::Vector2f({ 400.f, 400.f }));
  circle.setFillColor(sf::Color::Transparent);

  LightTexture.clear(sf::Color(0, 0, 0));

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
  system.AddLightObject({ 550, 60 }, { 200, 90 }, sf::Color::Black);
  //system.AddLightObject({ 200, 200 }, { 80, 180 }, sf::Color::Black);
  //system.AddLightObject({ 500, 550 }, { 80, 40 }, sf::Color::Black);
  system.AddLightObject({ 700, 500 }, { 80, 40 }, sf::Color::Black);
  //system.AddLightObject({ 300, 650 }, { 30, 25 }, sf::Color::Black);

  system.AddLight({ 400, 400 }, 400, sf::Color::Blue);

  std::size_t tri_to_draw = 0;
  window.setKeyRepeatEnabled(true);

  float CircleRadius = 200.f;

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

  sf::Glsl::Vec4 color = { 255, 255, 255, 100 };
  sf::Glsl::Vec2 center = { 400.f, 400.f };
  
  LightFragShader.setUniform("color", color);
  LightFragShader.setUniform("center", center);
  LightFragShader.setUniform("radius", CircleRadius);
  LightFragShader.setUniform("expand", 0.2f);
  LightFragShader.setUniform("windowHeight", static_cast<float>( window.getSize().y ));

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

  sf::Vector2f Light1Center(100.f, 600.f);
  sf::Vector2f Light2Center(450.f, 150.f);

  sf::Vector2f Light1Pos(200.f, 400.f);
  sf::Vector2f Light2Pos(600.f, 600.f);

  float Light1Theta = 0;
  float Light2Theta = 0;

  float Light1PathRad = 105.f;
  float Light2PathRad = 55.f;

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

  sf::Shader BlurShader;
  sf::RenderTexture LightMapTexture;
  sf::RenderTexture BlurredShadowsTexture;
  sf::Sprite FinalShadowSprite;

  BlurredShadowsTexture.create(800, 800);
  BlurredShadowsTexture.clear(sf::Color::Transparent);
  LightMapTexture.create(800, 800);
  LightMapTexture.clear(sf::Color::Transparent);

  BlurShader.loadFromFile("BlurShader.fsh", sf::Shader::Fragment);

  sf::Sprite BlurredLightmap;
  //BlurredLightmap.setTexture(LightMapTexture.getTexture());

  FinalShadowSprite.setTexture(BlurredShadowsTexture.getTexture());

  sf::RenderStates BlurState;
  sf::RenderStates ObjectState;

  BlurState.blendMode = sf::BlendAdd;
  BlurState.shader = &BlurShader;

  ObjectState.blendMode = sf::BlendAdd;

  bool UseBlurredTexture = false;
  system.AttenuationRadius.setString("Light Attenuation: " + std::to_string(CircleRadius));

  srand(NULL);

  float x_pos { 0.f }, y_pos { 0.f }, width { 0.f }, height { 0.f };

  sf::Vector2f Light1Dir(1, 0);
  sf::Vector2f Light2Dir(1, 0);

  sf::Vector2f Light1PreviousPos = Light1Pos + Light1Dir * Light1PathRad;
  sf::Vector2f Light2PreviousPos = Light2Pos + Light2Dir * Light2PathRad;

  //render the lit light texture to this and use it as a mash for the rest of the scene
  //the brighter the color here, the brighter to make the pixel
  sf::RenderTexture ShadowMask;
  ShadowMask.create(800, 800);
  ShadowMask.clear(sf::Color::Transparent);

  sf::RenderTexture RenderedScene;
  RenderedScene.create(800, 800);
  RenderedScene.clear(sf::Color::Transparent);

  sf::RectangleShape ShadowMaskQuad;
  ShadowMaskQuad.setSize({ 800, 800 });

  sf::Shader ShadowMaskShader;
  ShadowMaskShader.loadFromFile("MaskShader.fsh", sf::Shader::Fragment);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Button::Left) {
          x_pos = (float)sf::Mouse::getPosition(window).x;
          y_pos = (float)sf::Mouse::getPosition(window).y;
          width = (float)( rand() % 100 + 50 );
          height = (float)( rand() % 100 + 50 );
          system.AddLightObject({ x_pos, y_pos }, { width, height }, sf::Color::Black);
        }
      } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Space) {
          system.theta = 0;
          system.AdvanceSweep({ 400.f, 400.f }, 400.f);
        }

        else if (event.key.code == sf::Keyboard::Up) {
          CircleRadius += 10;
          system.AttenuationRadius.setString("Light Attenuation: " + std::to_string(CircleRadius));
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          //LightSystemState.texture = &LightTexture.getTexture();
        }

        else if (event.key.code == sf::Keyboard::Down) {
          CircleRadius -= 10;
          system.AttenuationRadius.setString("Light Attenuation: " + std::to_string(CircleRadius));
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          //LightSystemState.texture = &LightTexture.getTexture();
        }

        else if (event.key.code == sf::Keyboard::L) {
          system.UseLookupTable = !system.UseLookupTable;
        }

        else if (event.key.code == sf::Keyboard::Q) {
          Intensity += 5;
          for (auto & c : Colors)
            c.a = Intensity;
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
        } else if (event.key.code == sf::Keyboard::A) {
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
          current_color = ( current_color + 1 ) % Colors.size();
          RecreateLightTexture(LightTexture, LightFragShader, Colors[current_color], CircleRadius, circle);
          //LightSystemState.texture = &LightTexture.getTexture();
        } //c clicked, change colors
        else if (event.key.code == sf::Keyboard::P) {
          paused = !paused;
          system.StatusText.setString(paused ? "Paused" : "Playing");
          system.StatusText.setFillColor(paused ? sf::Color::Red : sf::Color::Green);
        } else if (event.key.code == sf::Keyboard::Num2) {
          secondlight = !secondlight;
          system.SecondLightText.setString(secondlight ? "Second Light: Enabled" : "Second Light: Disabled");
          //Light1Theta = 0;
        } else if (event.key.code == sf::Keyboard::Num3) {
          thirdlight = !thirdlight;
          system.ThirdLightText.setString(thirdlight ? "Third Light: Enabled" : "Third Light: Disabled");
          //Light2Theta = 0;
        }
      }
    }

    LightSystemState.blendMode = sf::BlendAdd;

    window.clear();

    if (!paused) {
      Light1Theta += 2 * PI / MAX_CIRCLE_ANGLE;
      Light2Theta += 2 * PI / MAX_CIRCLE_ANGLE;

      if (Light1Theta >= 2 * PI)
        Light1Theta = 0;
      if (Light2Theta >= 2 * PI)
        Light2Theta = 0;

#ifdef USE_LOOKUP_TABLE
      Light1Dir = sf::Vector2f(fastcos(Light1Theta), fastsin(Light1Theta));
      Light2Dir = sf::Vector2f(fastcos(Light2Theta), fastsin(Light2Theta));
#else
      Light1Dir = sf::Vector2f(std::cos(Light1Theta), std::sin(Light1Theta));
      Light2Dir = sf::Vector2f(std::cos(Light2Theta), std::sin(Light2Theta));
#endif


      Light1Pos = Light1Center + ( Light1Dir * Light1PathRad );
      Light2Pos = Light2Center + ( Light2Dir * Light2PathRad );

      sf::VertexArray l1(sf::Lines, 2);
      l1[0].position = Light1PreviousPos; l1[1].position = Light1Pos;
      l1[0].color = sf::Color::Yellow; l1[1].color = sf::Color::Yellow;

      sf::VertexArray l2(sf::Lines, 2);
      l2[0].position = Light1PreviousPos; l2[1].position = Light1Pos;
      l2[0].color = sf::Color::Yellow; l2[1].color = sf::Color::Yellow;

      Light1PreviousPos = Light1Pos;
      Light2PreviousPos = Light2Pos;

      system.RefreshFrame();
      system.AdvanceSweep(sf::Vector2f(sf::Mouse::getPosition(window)), CircleRadius);
      if (secondlight) {
        system.AdvanceSweep(Light1Pos, CircleRadius);
        system.Light1Path.push_back(l1);
      } else
        system.Light1Path.clear();
      if (thirdlight) {
        system.AdvanceSweep(Light2Pos, CircleRadius);
        system.Light2Path.push_back(l2);
      } else
        system.Light2Path.clear();
    }

    ShadowMask.clear(sf::Color::Transparent);
    RenderedScene.clear(sf::Color::Transparent);
    BlurredShadowsTexture.clear(sf::Color::Transparent);


    //render normal geometry
    system.RenderObjectsOnly(RenderedScene);

    //render light map
    system.RenderLightMap(ShadowMask, LightSystemState);
    ShadowMask.display();

    //use a crappy convolution matrix to blur the light map (use Gaussian instead?)
    BlurShader.setUniform("SCENE", ShadowMask.getTexture());
    BlurShader.setUniform("windowHeight", 800.f);
    BlurShader.setUniform("radius", CircleRadius);
    BlurredLightmap.setTexture(ShadowMask.getTexture());

    BlurredShadowsTexture.draw(BlurredLightmap, BlurState);
    BlurredShadowsTexture.display();

    window.clear(sf::Color::Black);

    //Use the lightmap and the normal geometry in the MaskShader to render lights + geometry
    ShadowMaskShader.setUniform("MaskTexture", BlurredShadowsTexture.getTexture());
    ShadowMaskShader.setUniform("SceneTexture", RenderedScene.getTexture());
    ShadowMaskShader.setUniform("MinimumIntensity", 1.0f);
    ShadowMaskShader.setUniform("LightHue", sf::Glsl::Vec4(Colors[current_color]));
    ShadowMaskShader.setUniform("HueIntensity", 0.3f);
    ShadowMaskShader.setUniform("MaximumIntensity", 10.0f);
    ShadowMaskQuad.setTexture(&BlurredShadowsTexture.getTexture());
    window.draw(ShadowMaskQuad, &ShadowMaskShader);
    window.display();
  }

  return 0;
}