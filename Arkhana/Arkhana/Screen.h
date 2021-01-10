#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Screen
{
public:
	Screen();
	~Screen(){}
	virtual void Draw() {}
	virtual void Update(Time t) {}
	virtual void MouseMoved(Vector2f mousePos) {}
	virtual void MouseClicked(Vector2f mousePos) {}
	virtual void MouseReleased(Vector2f mousePos) {}
protected:
	RenderWindow* window;
};

