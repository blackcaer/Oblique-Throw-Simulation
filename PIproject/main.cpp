// blackcaer
#define _USE_MATH_DEFINES

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Projectile.h"
#include "Dir.h"

int main()
{
	// Window, views
	sf::RenderWindow window(sf::VideoMode(1280, 720), "FluidSimulator", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	sf::View view(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), sf::Vector2f(1280, 720));
	view.setViewport(sf::FloatRect(0,0,1,1));
	
	
	// Shapes
	auto rect = sf::RectangleShape(sf::Vector2f(20000, 20));
	rect.setFillColor(sf::Color::Green);
	rect.setPosition(-2000,0);

	auto rect00 = sf::RectangleShape(sf::Vector2f(40, 80));
	rect00.setFillColor(sf::Color::Blue);
	rect00.setPosition(0, 0);

	

	float unit_to_px = 100;

	float
		radius = 10.f,
		h=Dir::up*(radius+1.f),
		angle = M_PI / 4.f,
		v = 8.f,
		vx = v * cos(angle) * Dir::right,
		vy = v * sin(angle) * Dir::up,
		g = 9.81f,
		ax = 0.f,
		ay = g * Dir::down;

	Projectile ball = Projectile(radius,0.f,h);

	sf::Clock deltaClock;
	sf::Time deltaTime;
	sf::String playerInput;
	sf::Text playerText;
	bool running = true;
	bool simulate_movement = false;
	int counter = 0;
	float sum = 0.f;

	sf::Event event;
	while (running)
	{
		deltaTime = deltaClock.restart();
		float dt = deltaTime.asSeconds();
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{

			}
			else if (event.type == sf::Event::TextEntered)
			{
				playerInput += event.text.unicode;
				playerText.setString(playerInput);
				printf("%c", event.text.unicode);

				if (event.text.unicode == 's')
					simulate_movement = true;
				if(event.text.unicode == 'x')
					view.zoom(2.f);
				if (event.text.unicode == 'z')
					view.zoom(0.5f);
				if (event.text.unicode == 'c')
					view.setCenter(ball.getShape()->getPosition());
				if (event.text.unicode == 'r')
					ball.getShape()->setPosition(0,0);
				if (event.text.unicode == 'g')
				{
					vx = 0;
					vy = 0;
					ax = 0;
					ay = 0;
					simulate_movement = false;
				}
			}
		}

		window.clear(sf::Color(100, 100, 100, 255));
		window.setView(view);

		sum += dt;
		// FPS counter
		if (sum > 1.f)
		{
			//printf("FPS: %f\n", counter * 1.f / sum);
			counter = 0;
			sum = 0;
			printf("==========\nx=%d, y=%d\n\n", int(ball.getShape()->getPosition().x), (int)(ball.getShape()->getPosition().y));
			printf("vx=%d, vy=%d\n",(int)vx, (int)vy);
		}

		if (simulate_movement)
		{
			vx += ax * dt;
			vy += ay * dt;

			float xoffset = dt * vx * unit_to_px;
			float yoffset = dt * vy * unit_to_px;

			// Check for colisions
			auto position = ball.getShape()->getPosition();
			auto border_y = rect.getPosition().y;

			if (position.y + radius + yoffset >= border_y)
			{
				// End movement, colision with the ground
				float to_col_y = fabs(border_y - position.y);
				float aoa = atan(vy / vx); // angle of attack
				float to_col_x = to_col_y / tan(aoa);
				ball.move(to_col_x - radius, to_col_y - radius);
				simulate_movement = false;
			}
			else {
				ball.move(xoffset, yoffset);
			}	
		}

		window.draw(rect);
		window.draw(rect00);
		//window.draw(playerText);
		window.draw(*ball.getShape());
		window.display();

		counter++;
	}

	return 0;
}