#include "ProjectileSimulator.h"


ProjectileSimulator::ProjectileSimulator(ProjectileSimulatorArgs args) :
	START_radius(args.ball_radius),
	unit_to_px(args.unit_to_px),
	VIEW_CHANGE(args.view_change),
	TRACER_RADIUS(args.tracer_radius),
	COLOR_BALL(args.color_ball),
	COLOR_BACKGROUND(args.color_background),
	COLOR_GROUND(args.color_ground),
	STATS_FILE(args.stats_file),

	window(sf::VideoMode(1536, 960), "FluidSimulator", sf::Style::Default, sf::ContextSettings(32)),
	view_game(sf::FloatRect(0, 0, 1080, 720)),//sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), sf::Vector2f(1280,720)), 
	view_controls(sf::Vector2f(0, 0), sf::Vector2f(1280, 192)),
	ground(sf::Vector2f(400000, 200000)),
	rect00(sf::Vector2f(5, 5))
{
	window.setPosition(sf::Vector2i(120,20));
	radius = START_radius;
	h_start = START_h;
	angle = START_angle;
	v_start = START_v_start;
	g = START_g;

	if (!font_main.loadFromFile("arial.ttf"))
	{
		printf("FONT ERROR");
		exit(0);
	}

	tracers = std::vector <sf::CircleShape*>();
	widgets_in = std::vector <Widget*>();
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	window.clear(sf::Color(255, 0, 0));

	//======== Shapes
	top_bar = sf::RectangleShape(sf::Vector2f(1280, 300));
	top_bar.setFillColor(sf::Color(90, 90, 90));
	top_bar.setPosition(-640, -100);
	
	ground.setFillColor(COLOR_GROUND);
	ground.setPosition(-70000, 0);

	rect00.setFillColor(sf::Color::Blue);
	rect00.setPosition(0, 0);

	create_widgets();

	this->ball = Projectile(radius, 0.f, h_start + radius, COLOR_BALL);
	ball.set_zero_coordinates();
	reset();

	view_controls.setViewport(sf::FloatRect(0, 0, 1, 0.2));
	view_game.setViewport(sf::FloatRect(0, 0.2, 1, 1));

}

void ProjectileSimulator::prep_text(sf::Text* text,int size,sf::Color color)
{
	text->setFont(font_main);

	text->setCharacterSize(size); // in pixels, not points
	//text->setPosition(200, 100);

	text->setFillColor(color);

	//text->setStyle(sf::Text::Bold | sf::Text::Underlined);
}

float ProjectileSimulator::deg_to_rad(float angle_deg)
{
	return angle_deg / 180.f * M_PI;
}

void ProjectileSimulator::reset()
{
	vx = v_start * cos(deg_to_rad(angle)) * Dir::right;
	vy = v_start * sin(deg_to_rad(angle)) * Dir::up;
	ax = 0.f;
	ay = g * Dir::down;

	ball.getShape()->setPosition(0.f, Dir::up * (h_start + radius));
	ball.set_zero_coordinates();
	if(follow_ball)
		center_view();

	// Calculate static variables
	Z = v_start * v_start * sin(2.f * deg_to_rad(angle)) / fabs(ay);
	Hmax = h_start + fabs(vy * vy / (2 * ay));
	th = fabs(vy / ay);

	// delete tracers
	for (auto i = 0; i < tracers.size(); i++)
	{
		delete tracers[i];
	}
	tracers.clear();

	update_static_widgets();

}

void ProjectileSimulator::print_info_to_console()
{
	sec_elapsed += deltaTime.asSeconds();
	// FPS counter
	if (sec_elapsed >= 1.f)
	{
		//printf("FPS: %f\n", counter * 1.f / sum);
		fps_per_sec = 0;
		sec_elapsed = 0;
		printf("==========\nx=%d, y=%d\n\n",
			int(ball.getShape()->getPosition().x),
			(int)(ball.getShape()->getPosition().y));

		printf("vx=%d, vy=%d\n", (int)vx, (int)vy);
	}
}

void ProjectileSimulator::center_view()
{
	view_game.setCenter(ball.getShape()->getPosition());
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

bool ProjectileSimulator::check_handle_collision(float* xoffset, float* yoffset)
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
		else // Xdddd to ogółem jakiś nieśmieszny żart TO POWINNO DZIAŁAĆ
		{
			float aoa = atan(vy / vx); // angle of attack
			float to_ground_x = to_ground_y / tan(aoa);
			//ball.move(to_ground_x - radius, to_ground_y - radius);
			//*xoffset = to_ground_x - radius;

			*xoffset = 0;	// hehe fix
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

	if(follow_ball)
		center_view();
}

void ProjectileSimulator::trace()
{
	sf::CircleShape* tracer = new sf::CircleShape(TRACER_RADIUS);
	tracer->setFillColor(tracer_color);
	tracer->setOrigin(TRACER_RADIUS, TRACER_RADIUS);
	tracer->setPosition(this->ball.getShape()->getPosition());
	tracers.push_back(tracer);
}

void ProjectileSimulator::_draw_widget(Widget* widget)
{
	for (int i = 0; i < widget->to_draw.size(); i++)
	{
		window.draw(*(widget->to_draw[i]));
	}
}

void ProjectileSimulator::_draw_widgets()
{
	window.draw(top_bar);
	for (auto i = 0; i < widgets_in.size(); i++)
	{
		_draw_widget(widgets_in[i]);
	}

	for (auto i = 0; i < widgets_other.size(); i++)
	{
		_draw_widget(widgets_other[i]);
	}

	for (auto i = 0; i < widgets_static.size(); i++)
	{
		_draw_widget(widgets_static[i]);
	}
}

void ProjectileSimulator::create_widgets()
{
	// ============== Widgets for entering data
	auto space = 170;
	auto startx = -630;
	int height = 96;
	widgets_in.clear();
	Widget* widget_v0 = new Widget(startx				, -5-height , 160, height, "V0=");
	Widget* widget_alpha = new Widget(startx + space * 1, -5 -height , 160, height, "alfa=");
	Widget* widget_h = new Widget(startx + space * 2	, -5 -height , 160, height, "h=");
	Widget* widget_g = new Widget(startx + space * 3	, -5 -height , 160, height, "g=");

	widget_v0->bind_variable(&v_start);
	widget_alpha->bind_variable(&angle);
	widget_h->bind_variable(&h_start);
	widget_g->bind_variable(&g);

	widget_v0->update_widget();
	widget_alpha->update_widget();
	widget_h->update_widget();
	widget_g->update_widget();

	//Widget* widget4 = new Widget(startx + space * 4, -40, 160, 80,"?=");
	//Widget* widget5 = new Widget(startx + space * 5, -40, 160, 80,"?=");
	//Widget* widget6 = new Widget(startx + space * 6, -40, 160, 80,"?=");

	widgets_in.push_back(widget_v0);
	widgets_in.push_back(widget_alpha);
	widgets_in.push_back(widget_h);
	widgets_in.push_back(widget_g);
	//widgets.push_back(widget4);
	//widgets.push_back(widget5);
	//widgets.push_back(widget6);


	// ============== Widgets for displaying real-time data
	Widget* widget_x = new Widget(startx			, 5, 160, height, "X=");
	Widget* widget_y = new Widget(startx + space * 1, 5, 160, height, "Y=");
	Widget* widget_vx = new Widget(startx + space * 2, 5, 160, height, "Vx=");
	Widget* widget_vy = new Widget(startx + space * 3, 5, 160, height, "Vy=");

	widgets_other.push_back(widget_x);
	widgets_other.push_back(widget_y);
	widgets_other.push_back(widget_vx);
	widgets_other.push_back(widget_vy);

	widget_x->bind_variable(&ball_x);
	widget_y->bind_variable(&ball_y);
	widget_vx->bind_variable(&vx);
	widget_vy->bind_variable(&vy);

	// ============== Widgets for displaying static data

	Widget* widget_Z = new Widget(20+startx + space * 4			, -5 -height, 160, height, "Zmax=");
	Widget* widget_Hmax = new Widget(20 + startx + space * 5	, -5 -height, 160, height, "Hmax=");
	Widget* widget_th = new Widget(20 + startx + space * 6		, -5 -height, 160, height, "th=");
	//Widget* widget_vy = new Widget(startx + space * 3, 0, 160, height, "Vy=");

	widgets_static.push_back(widget_Z);
	widgets_static.push_back(widget_Hmax);
	widgets_static.push_back(widget_th);
	//widgets_other.push_back(widget_vy);

	widget_Z->bind_variable(&Z);
	widget_Hmax->bind_variable(&Hmax);
	widget_th->bind_variable(&th);

}

void ProjectileSimulator::update_static_widgets()
{
	for (int i = 0; i < widgets_static.size(); i++)
	{
		widgets_static[i]->update_widget();
	}
}

void ProjectileSimulator::handle_moving_view(sf::Event event)
{
	auto center = view_game.getCenter();
	if (event.key.code == sf::Keyboard::Up)
		view_game.setCenter(center.x, center.y + VIEW_CHANGE * Dir::up);
	if (event.key.code == sf::Keyboard::Down)
		view_game.setCenter(center.x, center.y + VIEW_CHANGE * Dir::down);
	if (event.key.code == sf::Keyboard::Right)
		view_game.setCenter(center.x + VIEW_CHANGE * Dir::right, center.y);
	if (event.key.code == sf::Keyboard::Left)
		view_game.setCenter(center.x + VIEW_CHANGE * Dir::left, center.y);
}

void ProjectileSimulator::handle_tab()
{
	if (simulate_movement)
		return;
	if (focus_number == -1)
	{
		focus_number = 0;
		widgets_in[0]->toggle_focus();
	}
	else
	{
		widgets_in[focus_number]->update_variable(); // update previous widget
		widgets_in[focus_number]->toggle_focus(); // turn it OFF

		focus_number++;

		if (focus_number >= widgets_in.size())
		{
			focus_number = -1;	// This was last widget, set to -1
			reset(); // So you don't have to start two times for changes to take place
		}
		else
			widgets_in[focus_number]->toggle_focus(); // turn ON new widget

		
	}
}

void ProjectileSimulator::handle_letters(sf::Event event)
{
	if (event.text.unicode == 's')
		simulate_movement = true;
	if (event.text.unicode == 'x')
		view_game.zoom(2.f);
	if (event.text.unicode == 'z')
		view_game.zoom(0.5f);
	if (event.text.unicode == 'c')
		view_game.setCenter(ball.getShape()->getPosition());
	if (event.text.unicode == 'r')
	{
		ball.getShape()->setPosition(0.f, radius * Dir::up);
		reset();
	}
	if (event.text.unicode == 'v')
		follow_ball = !follow_ball;

	if (event.text.unicode == 'g')
	{
		simulate_movement = false;
	}
}

void ProjectileSimulator::handle_entering_numbers(sf::Event event)
{
	if (focus_number != -1 and ((
		event.text.unicode >= 48 and
		event.text.unicode <= 57) or
		event.text.unicode == '.'
		))
	{
		auto current_text = widgets_in[focus_number]->get_user_text();
		widgets_in[focus_number]->set_user_text(current_text + (char)(event.text.unicode));
	}
}

void ProjectileSimulator::handle_event(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::Closed:

		// end the program
		running = false;
		break;
	case sf::Event::Resized:

		break;
	case sf::Event::KeyPressed:

		handle_moving_view(event);

		if (event.key.code == sf::Keyboard::Tab)
			handle_tab();

		if (
			focus_number != -1 and
			(event.key.code == sf::Keyboard::BackSpace or
				event.key.code == sf::Keyboard::Delete)
			)
		{
			widgets_in[focus_number]->delete_last_char();
		}
		break;

	case sf::Event::TextEntered:
		printf("%c", event.text.unicode);
		handle_entering_numbers(event);
		if (focus_number == -1)
			handle_letters(event);

		break;
	default:
		break;
	}

}

void ProjectileSimulator::update_real_time_widgets()
{
	ball_x = ball.getShape()->getPosition().x - ball.x_zero;
	ball_y = ball.getShape()->getPosition().y - ball.y_zero;
	for (int i = 0; i < widgets_other.size(); i++)
	{
		widgets_other[i]->update_widget();
	}

}

void ProjectileSimulator::game_loop()
{


	sf::Text text1 = sf::Text();
	prep_text(&text1, 32, sf::Color::Black);
	//text1.setPosition();

	float time_for_tracer_s = 0.0f;
	sf::Event event;

	while (running)
	{
		deltaTime = deltaClock.restart();

		while (window.pollEvent(event))
		{
			handle_event(event);
		}

		window.setView(view_game);
		window.clear(COLOR_BACKGROUND);

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
		

		update_real_time_widgets();

		//============ Drawing 
		
		// Widgets
		window.setView(view_controls);

		_draw_widgets();

		window.setView(view_game);

		// Tracers
		for (auto i = 0; i < tracers.size(); i++)
		{
			window.draw(*tracers[i]);
		}

		window.draw(ground);
		window.draw(rect00);
		window.draw(*(ball.getShape()));

		window.display();

		fps_per_sec++;
	}

}

