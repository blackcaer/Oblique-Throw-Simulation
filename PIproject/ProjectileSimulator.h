#pragma once
#define _USE_MATH_DEFINES
#define TOLERANCE_F 1e-6


#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Projectile.h"
#include "Dir.h"
#include "Widget.h"

struct ProjectileSimulatorArgs {
	std::string stats_file = "";
	sf::Color color_ball = sf::Color::Red;
	sf::Color color_background = sf::Color(110, 110, 110, 255);
	sf::Color color_ground = sf::Color::Green;
	float unit_to_px = 100.f;
	float ball_radius = 16.f;
	float view_change = 30.f;;
	float tracer_radius = 6.f;
};

class ProjectileSimulator
{
public:
	bool running = true;

	ProjectileSimulator(ProjectileSimulatorArgs args);

	void game_loop();


private:
	const std::string STATS_FILE;
	sf::Color COLOR_BALL;
	sf::Color COLOR_BACKGROUND;
	sf::Color COLOR_GROUND;

	sf::Font font_main;
	sf::RenderWindow window;
	sf::View view_game;
	sf::View view_controls;
	sf::RectangleShape ground;
	sf::RectangleShape rect00;
	sf::RectangleShape top_bar;
	Projectile ball;
	std::vector <Widget*> widgets_in;
	std::vector <Widget*> widgets_other;
	std::vector <Widget*> widgets_static;

	bool follow_ball = true;

	float VIEW_CHANGE;
	float unit_to_px;
	bool simulate_movement = false;
	sf::Time deltaTime;
	sf::Clock deltaClock;

	//tracer
	const float TRACER_RADIUS;
	std::vector <sf::CircleShape*> tracers;
	float tracer_interval = 0.03f;
	sf::Color tracer_color = sf::Color::Blue;

	int focus_number = -1;

	const float START_radius;
	const float START_h = 0.f;
	const float START_angle = 45.f;
	const float START_v_start = 8.f;
	const float START_g = 9.81f;

	float h_start; // Can be set by user
	float angle; // Can be set by user
	float v_start; // Can be set by user
	float g; // Can be set by user

	float ball_x = 0.f;
	float ball_y = 0.f;

	float radius;
	float vx;
	float vy;
	float ax;
	float ay;

	float Z;
	float Hmax;
	float th;

	int fps_per_sec = 0;
	float sec_elapsed = 0.f;

	static float deg_to_rad(float angle_deg);

	void prep_text(sf::Text* text, int size, sf::Color color);

	void print_info_to_console();

	void handle_event(sf::Event);

	void reset();

	void center_view();

	void handle_moving_view(sf::Event event);

	void handle_tab();

	void handle_letters(sf::Event event);

	void handle_entering_numbers(sf::Event event);

	bool is_collision(float y);

	bool check_handle_collision(float*, float*);

	void move();

	void trace();

	void _draw_widget(Widget*);

	void _draw_widgets();

	void create_widgets();

	void update_real_time_widgets();

	void update_static_widgets();
};

