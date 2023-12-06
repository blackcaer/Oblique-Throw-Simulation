// blackcaer
#define _USE_MATH_DEFINES
#define TOLERANCE_F 1e-6

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Projectile.h"
#include "Dir.h"
#include "ProjectileSimulator.h"



int main()
{
	auto simulator = new ProjectileSimulator();
	simulator->game_loop();

	return 0;
}