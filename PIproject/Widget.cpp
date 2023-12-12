#include "Widget.h"

Widget::Widget(
	float x, 
	float y, 
	float width, 
	float height, 
	std::string static_text,
	std::string user_text,
	sf::Color color,
	std::string font_file
) :
	to_draw(),
	static_text(static_text),
	user_text(user_text)
{
	if (!font_main.loadFromFile(font_file))
	{
		printf("FONT ERROR");
		exit(0);
	}
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	rect_back = new sf::RectangleShape(sf::Vector2f(this->width, this->height));
	rect_back->setFillColor(color);
	text = new sf::Text();
	
	prep_text(text);
	setPosition(x, y);

	to_draw.push_back(rect_back);
	to_draw.push_back(text);
}

void Widget::setPosition(float x, float y)
{
	rect_back->setPosition(x, y); 
	text->setPosition(x,y);
	centerText();
}

void Widget::set_user_text(std::string user_text)
{
	this->user_text=user_text;
	_set_text();
}

void Widget::set_static_text(std::string static_text)
{
	this->static_text=static_text;
	_set_text();
}

void Widget::centerText()
{
	auto w = this->text->getLocalBounds().width;
	auto h = this->text->getLocalBounds().height;
	this->text->setPosition(x + (width - w) / 2.f, y + (height - h) / 2.f);
	//this->text->setPosition(x + (width) / 2.f, y + (height) / 2.f);
}

void Widget::toggle_focus()
{
	has_focus = !has_focus;
	if (has_focus == true)
	{
		text->setFillColor(COLOR_ACTIVE);
	}
	else
	{
		text->setFillColor(COLOR_NORMAL);

	}
}

bool Widget::get_focus()
{
	return has_focus;
}

std::string Widget::get_user_text()
{
	return user_text;
}

void Widget::prep_text(sf::Text* text)
{
	text->setFont(font_main);
	text->setCharacterSize(26); // in pixels, not points
	text->setFillColor(sf::Color::Blue);
	text->setStyle(sf::Text::Bold);
	centerText();
}


void Widget::_set_text()
{
	this->text->setString(static_text + user_text);
	centerText();
}