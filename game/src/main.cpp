#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "console.h"
#include "shaker.h"

constexpr auto S_WIDTH = 400;
constexpr auto S_HEIGHT = 400;

int main()
{
	cs::HideConsole();
	//cs::ShowConsole();

	std::vector<sf::Color> palette
	{
		sf::Color(9,  38,  58),
		sf::Color(24,  95, 140),
		sf::Color(149, 194, 199),
		sf::Color(235, 213, 104),
		sf::Color(241, 113,  65),
	};

	auto settings = sf::ContextSettings::ContextSettings(0, 0, 8);
	sf::RenderWindow& window = sf::RenderWindow(sf::VideoMode(S_WIDTH, S_HEIGHT), "Premake!!!", 7U, settings);
	sf::FloatRect camRect(-S_WIDTH / 2, -S_HEIGHT / 2, S_WIDTH, S_HEIGHT);
	sf::Transform camMatrix = sf::Transform::Identity;

	sf::View mainCamera(camRect);
	window.setView(mainCamera);

	sf::CircleShape circle(50.0);
	circle.setPosition(0, 0);
	circle.setOrigin(50, 50);
	circle.setTextureRect(sf::IntRect(0, 0, 1, 1));
	circle.setFillColor(palette[1]);

	std::vector<sf::RectangleShape> rects;

	sf::RectangleShape rect1(sf::Vector2f(50, 100));
	rect1.setPosition(100, 0);
	rect1.setOrigin(25, 50);
	rect1.setFillColor(palette[2]);
	rect1.setTextureRect(sf::IntRect(0, 0, 1, 1));
	rects.push_back(rect1);

	sf::RectangleShape rect2(sf::Vector2f(50, 100));
	rect2.setPosition(-100, 0);
	rect2.setOrigin(25, 50);
	rect2.setFillColor(palette[2]);
	rect2.setTextureRect(sf::IntRect(0, 0, 1, 1));
	rects.push_back(rect2);

	sf::RectangleShape rect3(sf::Vector2f(100, 50));
	rect3.setPosition(0, 100);
	rect3.setOrigin(50, 25);
	rect3.setFillColor(palette[2]);
	rect3.setTextureRect(sf::IntRect(0, 0, 1, 1));
	rects.push_back(rect3);

	sf::RectangleShape rect4(sf::Vector2f(100, 50));
	rect4.setPosition(0, -100);
	rect4.setOrigin(50, 25);
	rect4.setFillColor(palette[2]);
	rect4.setTextureRect(sf::IntRect(0, 0, 1, 1));
	rects.push_back(rect4);


	sf::RectangleShape rectcentre(sf::Vector2f(100, 100));
	rectcentre.setPosition(400, 300);
	rectcentre.setOrigin(50, 50);
	rectcentre.setFillColor(sf::Color(0, 0, 0, 0));
	rectcentre.setOutlineColor(sf::Color::Green);
	rectcentre.setOutlineThickness(1);

	sf::Font font;
	if (!font.loadFromFile("assets/arial.ttf"))
	{
		cs::Print("it kinda broke!");
	}
	sf::Text text("H", font, 50U);
	auto bounds = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));

	sf::Clock clock;
	float t = -4.0f;

	Shaker shaker((int)time(NULL));

	float shakeScale = 0;

	sf::Shader shader;
	std::string frag = R"(
        uniform float g_time;
        uniform float o_rand;
        uniform float o_ratio;

        vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

        float snoise(vec2 v){
          const vec4 C = vec4(0.211324865405187, 0.366025403784439,
                   -0.577350269189626, 0.024390243902439);
          vec2 i  = floor(v + dot(v, C.yy) );
          vec2 x0 = v -   i + dot(i, C.xx);
          vec2 i1;
          i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
          vec4 x12 = x0.xyxy + C.xxzz;
          x12.xy -= i1;
          i = mod(i, 289.0);
          vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
          + i.x + vec3(0.0, i1.x, 1.0 ));
          vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
            dot(x12.zw,x12.zw)), 0.0);
          m = m*m ;
          m = m*m ;
          vec3 x = 2.0 * fract(p * C.www) - 1.0;
          vec3 h = abs(x) - 0.5;
          vec3 ox = floor(x + 0.5);
          vec3 a0 = x - ox;
          m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
          vec3 g;
          g.x  = a0.x  * x0.x  + h.x  * x0.y;
          g.yz = a0.yz * x12.xz + h.yz * x12.yw;
          return 130.0 * dot(m, g);
        }

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float n = snoise(vec2(uv.x + g_time * 2, (uv.y / o_ratio) + o_rand));
            float r = smoothstep(-0.1, 0.1, n);
            gl_FragColor = vec4(1, 1, 1, r) * gl_Color;
        }
        )";
	if (!shader.loadFromMemory(frag, sf::Shader::Type::Fragment))
	{
		cs::ShowConsole();
		cs::Print("Failed to load shader!");
	}

	sf::Shader screenShader;
	std::string screenFrag = R"(
        uniform vec2 resolution;

        void main() {
            gl_FragColor = vec4(gl_FragCoord.xy / resolution, 0, 1);
        }
        )";
	if (!screenShader.loadFromMemory(screenFrag, sf::Shader::Type::Fragment))
	{
		cs::ShowConsole();
		cs::Print("Failed to load screen shader!");
	}

	screenShader.setUniform("resolution", sf::Vector2f(S_HEIGHT, S_WIDTH));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::Resized)
			{
				float w = event.size.width;
				float h = event.size.height;
				float screenRatio = w / h;

				h = S_HEIGHT;
				w = h * screenRatio;

				sf::FloatRect visibleArea(-w / 2, -h / 2, w, h);
				mainCamera = sf::View(visibleArea);
				window.setView(mainCamera);
			}

			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8)
				{
					text.setString(text.getString().substring(0, text.getString().getSize() - 1));
					auto bounds = text.getLocalBounds();
					text.setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
				}
				else 
				{
					text.setString(text.getString() + (char)event.text.unicode);
					auto bounds = text.getLocalBounds();
					text.setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
				}


			}
		}

		float dt = clock.restart().asSeconds();
		t += dt;

		if (t > 2)
		{
			shakeScale -= dt * 0.5;
			if (shakeScale < 0)
				shakeScale = 0;
		}
		else if (t > 1)
		{
			shakeScale += dt;
			if (shakeScale > 1)
				shakeScale = 1;
		}

		shader.setUniform("g_time", t);

		auto shaderState = sf::RenderStates::Default;
		shaderState.shader = &shader;

		auto normalState = sf::RenderStates::Default;

		window.clear(palette[0]);

		int i = 0;
		for (auto& rect : rects)
		{
			i++;
			shader.setUniform("o_rand", i * 127.128f);
			shader.setUniform("o_ratio", rect.getSize().x / rect.getSize().y);
			window.draw(rect, normalState);
		}
		if (t > 0)
		{
			circle.move(dt * sf::Vector2f(30, 0));
		}
		shader.setUniform("o_rand", 89746589);
		shader.setUniform("o_ratio", 1.0f);
		window.draw(circle, shaderState);
		//window.draw(rectcentre);

		window.draw(text);

		auto screenState = sf::RenderStates::Default;
		screenState.shader = &screenShader;

		//window.draw(sf::RectangleShape(sf::Vector2f(S_WIDTH, S_HEIGHT)), screenState);

		window.display();
	}
}