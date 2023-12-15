#pragma once
//#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Projectile
{
private:
	sf::CircleShape* shape;
	const sf::Color DEF_COLOR = sf::Color::Red;
	int a;
public:

	Projectile(sf::CircleShape* shape);

	Projectile(float r = 10.f, float x = 0.f, float y = 0.f);

	~Projectile();

	Projectile& operator=(const Projectile& t);

	sf::Shape* getShape();

	void move(float x, float y);

};

