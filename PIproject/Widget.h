#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Dir.h"

class Widget
{
public:
	sf::Font font_main;
	std::vector <sf::Drawable*> to_draw;
	sf::RectangleShape* rect_back;
	sf::Text* text;
	std::string static_text;
	std::string user_text;
	
	sf::Color COLOR_NORMAL = sf::Color::Black;
	sf::Color COLOR_ACTIVE = sf::Color::Yellow;

	float width;
	float height;
	float x;
	float y;
	
	Widget(float x,
		float y,
		float width,
		float height, 
		std::string static_text="",
		std::string user_text="",
		sf::Color color = sf::Color(186, 186, 186),
		std::string font_file = "arial.ttf");

	void prep_text(sf::Text* text);

	void setPosition(float, float);


	void set_user_text(std::string user_text);

	void set_static_text(std::string static_text);

	void centerText();

	void toggle_focus();

	bool get_focus();

	std::string get_user_text();

private:

	bool has_focus = false;

	void _set_text();
};

