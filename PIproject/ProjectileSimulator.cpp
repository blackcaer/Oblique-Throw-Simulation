#include "ProjectileSimulator.h"


// TODO: Przeniesc ground i rect00 do tablicy do pozniejszego draw
ProjectileSimulator::ProjectileSimulator() :
	window(sf::VideoMode(1280, 720), "FluidSimulator", sf::Style::Default, sf::ContextSettings(32)),
	view(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), sf::Vector2f(1280, 720)),
	ground(sf::Vector2f(20000, 20)),
	rect00(sf::Vector2f(5, 5))
{
	tracers = std::vector <sf::CircleShape*>();
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	view.setViewport(sf::FloatRect(0, 0, 1, 1));

	//======== Shapes
	//auto ground = sf::RectangleShape(sf::Vector2f(20000, 20));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(-2000, 0);

	//auto rect00 = sf::RectangleShape(sf::Vector2f(5, 5));
	rect00.setFillColor(sf::Color::Blue);
	rect00.setPosition(0, 0);

	reset();

	ball = Projectile(radius, 0.f, h);
}

void ProjectileSimulator::reset()
{
	radius = 10.f;
	h = Dir::up * (radius + 0.f);
	angle = M_PI / 4.f;
	v_start = 8.f;
	g = 9.81f;

	vx = v_start * cos(angle) * Dir::right;
	vy = v_start * sin(angle) * Dir::up;
	ax = 0.f;
	ay = g * Dir::down;

	ball.getShape()->setPosition(0.f,h);
	center_view();

	// delete tracers
	for (auto i = 0; i < tracers.size(); i++)
	{
		delete tracers[i];
	}
	tracers.clear();
}

void ProjectileSimulator::print_info_to_console()
{
	sum += deltaTime.asSeconds();
	// FPS counter
	if (sum > 1.f)
	{
		//printf("FPS: %f\n", counter * 1.f / sum);
		counter = 0;
		sum = 0;
		printf("==========\nx=%d, y=%d\n\n", int(ball.getShape()->getPosition().x), (int)(ball.getShape()->getPosition().y));
		printf("vx=%d, vy=%d\n", (int)vx, (int)vy);
	}
}

void ProjectileSimulator::handle_event(sf::Event event)
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
		sf::String playerInput;
		sf::Text playerText;
		playerInput += event.text.unicode;
		playerText.setString(playerInput);
		printf("%c", event.text.unicode);

		if (event.text.unicode == 's')
			simulate_movement = true;
		if (event.text.unicode == 'x')
			view.zoom(2.f);
		if (event.text.unicode == 'z')
			view.zoom(0.5f);
		if (event.text.unicode == 'c')
			view.setCenter(ball.getShape()->getPosition());
		if (event.text.unicode == 'r')
		{
			ball.getShape()->setPosition(0.f, radius * Dir::up);
			reset();
		}

		if (event.text.unicode == 'g')
		{
			simulate_movement = false;
		}
	}
}

void ProjectileSimulator::center_view()
{
	view.setCenter(ball.getShape()->getPosition());
}

bool ProjectileSimulator::is_collision(float y)
// Checks if ball on coordinate y is colliding the ground
{
	if (y >= ground.getPosition().y)
	{
		return true;
	}
	return false;
}

bool ProjectileSimulator::check_handle_collision(float* xoffset,float* yoffset)
{
	// Check for colisions
	auto position = ball.getShape()->getPosition();
	
	if (is_collision(position.y + radius + *yoffset))
	{
		// End movement, colision with the ground

		float to_ground_y = fabs(ground.getPosition().y - position.y);

		if (fabs(vx) <= TOLERANCE_F)
		{
			//ball.move(0.f, to_ground_y - radius);
			*xoffset = 0.f;
			*yoffset = to_ground_y - radius;
		}
		else
		{
			float aoa = atan(vy / vx); // angle of attack
			float to_ground_x = to_ground_y / tan(aoa);
			//ball.move(to_ground_x - radius, to_ground_y - radius);
			*xoffset = to_ground_x - radius;
			*yoffset = to_ground_y - radius;
		}
		simulate_movement = false;

		return true;
	}
	else {
		// normal movement, xoffset and yoffset should remain the same
		return false;
	}
}

void ProjectileSimulator::move()
{
	float dt = deltaTime.asSeconds();

	/*vx += ax * dt;
	vy += ay * dt;
	float xoffset = dt * vx * unit_to_px;
	float yoffset = dt * vy * unit_to_px;*/

	float xoffset = (vx * dt + ax * dt * dt / 2.f) * unit_to_px;
	float yoffset = (vy * dt + ay * dt * dt / 2.f) * unit_to_px;

	vx += ax * dt;
	vy += ay * dt;

	bool will_collide = check_handle_collision(&xoffset, &yoffset);
	if (will_collide)
	{
		vx = 0.f;
		vy = 0.f;

	}
	ball.move(xoffset, yoffset);

	center_view();
}

void ProjectileSimulator::trace()
{
	sf::CircleShape * tracer = new sf::CircleShape(TRACER_RADIUS);
	tracer->setFillColor(tracer_color);
	tracer->setPosition(this->ball.getShape()->getPosition());
	tracers.push_back(tracer);
}

void ProjectileSimulator::game_loop()
{
	sf::Event event;

	while (running)
	{
		deltaTime = deltaClock.restart();

		while (window.pollEvent(event))
		{
			handle_event(event);
		}

		window.clear(sf::Color(100, 100, 100, 255));
		window.setView(view);

		print_info_to_console();

		if (simulate_movement)
		{
			move();
			time_for_tracer_s += deltaTime.asSeconds();
			if (time_for_tracer_s >= tracer_interval)
			{
				time_for_tracer_s = 0.f;
				trace();
			}
		}


		for (auto i = 0; i < tracers.size(); i++)
		{
			window.draw(*tracers[i]);
		}

		window.draw(ground);
		window.draw(rect00);
		window.draw(*(ball.getShape()));
		//window.draw(playerText);
		window.display();

		counter++;
	}

}