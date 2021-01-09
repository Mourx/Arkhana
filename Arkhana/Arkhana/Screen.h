#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Screen
{
public:
	Screen();
	~Screen(){}
	virtual void Draw() {}
protected:
	RenderWindow* window;
};

