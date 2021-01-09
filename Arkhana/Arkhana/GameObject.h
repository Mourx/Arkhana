
#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class GameObject
{
public:
	GameObject();
	~GameObject() {}
	virtual void Draw(RenderWindow* w);
	virtual void SetPosition(Vector2f p) {
		pos = p;
		icon.setPosition(pos);
	}
protected:
	Sprite icon;
	Texture texIcon;
	Vector2f pos;
};

