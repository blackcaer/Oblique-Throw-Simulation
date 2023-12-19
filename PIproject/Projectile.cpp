#include "Projectile.h"

Projectile::Projectile(float r, float x, float y,sf::Color default_ball_color) : 
	DEF_COLOR(default_ball_color)
{
	sf::CircleShape* new_shape = new sf::CircleShape();
	new_shape->setFillColor(DEF_COLOR);
	new_shape->setRadius(r);
	new_shape->setPosition(x, y);
	new_shape->setOrigin(r, r);

	this->shape = new_shape;
}

Projectile::~Projectile()
{
	delete shape;
}

Projectile& Projectile::operator=(const Projectile& other)
{
	if (this != &other)
	{
		delete shape;

		shape = new sf::CircleShape(*other.shape);
		this->x_zero = other.x_zero;
		this->y_zero = other.y_zero;
	}

	return *this;
}

sf::Shape* Projectile::getShape()
{
	return shape;
}

void Projectile::move(float x, float y)
{
	shape->move(x, y);

}

void Projectile::set_zero_coordinates()
{
	x_zero = shape->getPosition().x;
	y_zero = shape->getPosition().y;
}
