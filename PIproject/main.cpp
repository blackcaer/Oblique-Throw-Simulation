// blackcaer

/*
* #define _USE_MATH_DEFINES
#define TOLERANCE_F 1e-6
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Projectile.h"
#include "Dir.h"
#include "Widget.h"*/

#include "ProjectileSimulator.h"

#include <sstream>
#include <iostream>
#include <fstream>

const std::string CONFIG_PATH = "config.txt";

void parse_args(std::string key, std::string value, ProjectileSimulatorArgs* args);
ProjectileSimulatorArgs get_config(std::string config_path);

int main()
{
	auto config = get_config(CONFIG_PATH);
	auto simulator = new ProjectileSimulator(config);
	simulator->game_loop();

	return 0;
}

ProjectileSimulatorArgs get_config(std::string config_path)
{
	ProjectileSimulatorArgs args;
	std::string file_content;
	std::string line;
	std::ifstream myfile(config_path);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			std::cout << line << '\n';
			file_content += line;
			file_content += '\n';
		}
		myfile.close();
	}

	std::istringstream is_file(file_content, 1);

	while (std::getline(is_file, line))
	{
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '='))
		{
			std::string value;
			if (std::getline(is_line, value))
				parse_args(key, value, &args);
		}
	}
	return args;
}

void parse_args(std::string key, std::string value, ProjectileSimulatorArgs* args)
{
	// nie jestem z tego dumny ale nie mam czasu na nic m¹drzejszego:
	if (key == "stats_file")
	{
		args->stats_file = value;
	}
	else if (key == "color_ball")
	{
		args->color_ball = sf::Color(std::stoul(value));
	}
	else if (key == "color_background")
	{
		args->color_background = sf::Color(std::stoul(value));

	}
	else if (key == "color_ground")
	{
		args->color_ground = sf::Color(std::stoul(value));

	}
	else if (key == "unit_to_px")
	{
		args->unit_to_px = std::stof(value);

	}
	else if (key == "ball_radius")
	{
		args->ball_radius = std::stof(value);

	}
	else if (key == "view_change")
	{
		args->view_change = std::stof(value);

	}
	else if (key == "tracer_radius")
	{
		args->tracer_radius = std::stof(value);

	}
	return;
}