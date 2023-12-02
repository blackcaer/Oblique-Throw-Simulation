// blackcaer
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Projectile.h"

int main()
{
	// Window, views
	sf::RenderWindow window(sf::VideoMode(1280, 720), "FluidSimulator", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	sf::View view(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), sf::Vector2f(1280, 720));
	view.setViewport(sf::FloatRect(0,0,1,1));
	
	
	// Shapes
	auto rect = sf::RectangleShape(sf::Vector2f(40, 80));
	rect.setFillColor(sf::Color::Green);
	rect.setPosition(300,300);

	auto rect00 = sf::RectangleShape(sf::Vector2f(40, 80));
	rect00.setFillColor(sf::Color::Blue);
	rect00.setPosition(0, 0);

	Projectile ball = Projectile();

	float h, v, vx, vy, a;

	float speed = 100;

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
			}
		}

		window.clear(sf::Color(100, 100, 100, 255));
		window.setView(view);

		sum += deltaTime.asSeconds();
		// FPS counter
		if (sum > 1.f)
		{
			//printf("FPS: %f\n", counter * 1.f / sum);
			counter = 0;
			sum = 0;
			printf("x=%d, y=%d\n\n", int(ball.getShape()->getPosition().x), (int)(ball.getShape()->getPosition().y));
		}

		if (simulate_movement)
		{
			float offset = deltaTime.asSeconds() * speed;
			ball.move(offset, offset);
		}

		window.draw(rect);
		window.draw(rect00);
		window.draw(playerText);
		window.draw(*ball.getShape());
		window.display();

		counter++;
	}

	return 0;
}