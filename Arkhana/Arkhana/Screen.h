#pragma once
#include <SFML/Graphics.hpp>
#include "enums.h"
#include "InfoPane.h"

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
	NEXT_SCREEN GetNextScreen() { return nextScreen; }
	NEXT_SCREEN GetType() { return type; }
	virtual void SetInfo(InfoPane* info) {}
protected:
	RenderTexture* window;
	NEXT_SCREEN nextScreen = NONE;
	NEXT_SCREEN type = NONE;
	String description = "";
};

