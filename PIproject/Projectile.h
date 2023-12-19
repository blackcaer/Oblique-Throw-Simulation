#pragma once
//#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Projectile
{
private:
	sf::CircleShape* shape;
	const sf::Color DEF_COLOR = sf::Color::Red;

public:
	float x_zero = 0.f;
	float y_zero = 0.f;

	Projectile(float r = 10.f, float x = 0.f, float y = 0.f, sf::Color default_ball_color = sf::Color::Red);

	~Projectile();

	Projectile& operator=(const Projectile& t);

	sf::Shape* getShape();

	void move(float x, float y);

	void set_zero_coordinates();


};

