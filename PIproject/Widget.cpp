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
	to_draw()
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

	set_static_text(static_text);
	set_user_text(user_text);

	prep_text(text);
	setPosition(x, y);

	to_draw.push_back(rect_back);
	to_draw.push_back(text);
}

void Widget::setPosition(float x, float y)
{
	rect_back->setPosition(x, y);
	text->setPosition(x, y);
	centerText();
}

void Widget::set_user_text(std::string user_text)
{
	this->user_text = user_text;
	_set_text();
}

void Widget::set_static_text(std::string static_text)
{
	this->static_text = static_text;
	_set_text();
}

void Widget::centerText()
{
	auto w = this->text->getLocalBounds().width;
	auto h_start = this->text->getLocalBounds().height;
	this->text->setPosition(x + (width - w) / 2.f, y + (height - h_start) / 2.f);
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

void Widget::delete_last_char()
{
	if (user_text.length() > 0)
	{
		this->set_user_text(user_text.substr(0, user_text.length() - 1));
	}
}

std::string Widget::get_user_text()
{
	return user_text;
}

void Widget::prep_text(sf::Text* text)
{
	text->setFont(font_main);
	text->setCharacterSize(pixel_size); // in pixels, not points
	text->setFillColor(COLOR_NORMAL);
	text->setStyle(sf::Text::Bold);
	centerText();
}

void Widget::_set_text()
{
	this->text->setString(static_text + user_text);
	centerText();
}

void Widget::bind_variable(float* var)
{
	binded_variable = var;
}

void Widget::update_variable()
{
	if (binded_variable != NULL and user_text != "")
	{
		*binded_variable = std::stof(user_text);
	}
}

void Widget::update_widget()
{
	if (binded_variable != NULL)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(precision) << *binded_variable;

		user_text = stream.str();
		this->_set_text();

	}
}