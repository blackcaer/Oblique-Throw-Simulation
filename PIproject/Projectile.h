#pragma once
//#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Projectile
{
private:
	sf::CircleShape* shape;
	const sf::Color DEF_COLOR = sf::Color::Red;

public:

	Projectile(sf::CircleShape* shape);

	Projectile(float x = 0.f,float y = 0.f,float r = 10.f);

	~Projectile();

	sf::Shape* getShape();

	void move(float x, float y);

};

