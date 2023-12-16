#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iomanip>
#include <sstream>

#include "Dir.h"

class Widget
{
public:
	int pixel_size = 26;
	int precision = 2;

	sf::Font font_main;
	std::vector <sf::Drawable*> to_draw;
	sf::RectangleShape* rect_back;
	sf::Text* text;
	std::string static_text;
	std::string user_text;

	sf::Color COLOR_NORMAL = sf::Color::Black;
	sf::Color COLOR_ACTIVE = sf::Color::Yellow;

	float* binded_variable = NULL;

	float width;
	float height;
	float x;
	float y;

	Widget(float x,
		float y,
		float width,
		float height,
		std::string static_text = "",
		std::string user_text = "",
		sf::Color color = sf::Color(186, 186, 186),
		std::string font_file = "arial.ttf"
	);

	void prep_text(sf::Text* text);

	void setPosition(float, float);

	void set_user_text(std::string user_text);

	void set_static_text(std::string static_text);

	void centerText();

	void toggle_focus();

	void delete_last_char();

	bool get_focus();

	void update_variable();

	void bind_variable(float*);

	void update_widget();

	std::string get_user_text();

private:

	bool has_focus = false;

	void _set_text();
};

