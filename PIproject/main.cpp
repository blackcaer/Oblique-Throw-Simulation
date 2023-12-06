// blackcaer
#define _USE_MATH_DEFINES
#define TOLERANCE_F 1e-6

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Projectile.h"
#include "Dir.h"



void reset_values(float* radius, float* h, float* angle, float* v, float* vx, float* vy, float* g, float* ax, float* ay)
{
	*radius = 10.f;
	*h = Dir::up * ((*radius) + 0.f);
	*angle = M_PI / 4.f;
	*v = 8.f;
	*vx = (*v) * cos(*angle) * Dir::right;
	*vy = (*v) * sin(*angle) * Dir::up;
	*g = 9.81f;
	*ax = 0.f;
	*ay = (*g) * Dir::down;

}

int main()
{
	// Window, views
	sf::RenderWindow window(sf::VideoMode(1280, 720), "FluidSimulator", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	sf::View view(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), sf::Vector2f(1280, 720));
	view.setViewport(sf::FloatRect(0,0,1,1));
	
	
	// Shapes
	auto ground = sf::RectangleShape(sf::Vector2f(20000, 20));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(-2000,0);

	auto rect00 = sf::RectangleShape(sf::Vector2f(5, 5));
	rect00.setFillColor(sf::Color::Blue);
	rect00.setPosition(0, 0);

	float unit_to_px = 100.f;

	float radius=0.f, h=0, angle=0, v=0, vx=0, vy=0, g=0, ax=0, ay=0;
	reset_values(&radius, &h, &angle, &v, &vx, &vy, &g, &ax, &ay);

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
				{
					ball.getShape()->setPosition(0.f, radius * Dir::up);
					reset_values(&radius, &h, &angle, &v, &vx, &vy, &g, &ax, &ay);
				}

				if (event.text.unicode == 'g')
				{
					
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
		//view.setCenter(ball.getShape()->getPosition());

		if (simulate_movement)
		{
			
			vx += ax * dt;
			vy += ay * dt;

			// sprawdzic z 0.5 offset, center,0.5 offset

			view.setCenter(ball.getShape()->getPosition());

			float xoffset = dt * vx * unit_to_px;
			float yoffset = dt * vy * unit_to_px;

			// Check for colisions
			auto position = ball.getShape()->getPosition();
			auto border_y = ground.getPosition().y;

			if (position.y + radius + yoffset >= border_y)
			{
				// End movement, colision with the ground

				float to_ground_y = fabs(border_y - position.y);

				if (fabs(vx) <= TOLERANCE_F)
					ball.move(0.f, to_ground_y - radius);	
				else
				{
					float aoa = atan(vy / vx); // angle of attack
					float to_ground_x = to_ground_y / tan(aoa);
					ball.move(to_ground_x - radius, to_ground_y - radius);
				}
				simulate_movement = false;
				
			}
			else {
				ball.move(xoffset, yoffset);
			}	
		}
		//view.setCenter(ball.getShape()->getPosition());
		window.draw(ground);
		window.draw(rect00);
		//window.draw(playerText);
		window.draw(*ball.getShape());
		window.display();

		counter++;
	}

	return 0;
}