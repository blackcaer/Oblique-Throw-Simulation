#include "Projectile.h"


Projectile::Projectile(sf::CircleShape* shape)
{
	if (shape == nullptr)
	{
		Projectile();
		return;
	}
	this->shape = shape;

}

Projectile::~Projectile()
{
	delete shape;
}

Projectile& Projectile::operator=(const Projectile& other)
{
	// Check for self-assignment
	if (this != &other)
	{
		// Delete the current shape
		delete shape;

		// Create a new shape and copy its properties
		shape = new sf::CircleShape(*other.shape);
	}

	return *this;
}

Projectile::Projectile(float r,float x, float y)
{
	sf::CircleShape* new_shape = new sf::CircleShape();
	new_shape->setFillColor(DEF_COLOR);
	new_shape->setRadius(r);
	new_shape->setPosition(x, y);
	new_shape->setOrigin(r,r);

	this->shape = new_shape;
}

sf::Shape* Projectile::getShape()
{
	return shape;
}

void Projectile::move(float x, float y)
{
	shape->move(x, y);
}

