
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
	Sprite* GetIcon() { return &icon; }
	virtual void Update(Time t);
	bool IsMoving() { return bMoving; }
	virtual void SetTarget(Vector2f tPos) { 
		targetPos = tPos;
		bMoving = true;
		xdir = (targetPos.x - pos.x) / moveDuration;
		ydir = (targetPos.y - pos.y) / moveDuration;

	}
	void SetHover(bool b) { bHover = b; }
	bool GetHover() { return bHover; }
	bool IsAtTarget() { return bAtTarget; }
	void ResetTarget() { bAtTarget = false; }
protected:
	Sprite icon;
	Texture texIcon;
	Vector2f pos;
	Vector2f targetPos;
	Shader shaderHover;
	bool bMoving = false;
	bool bHover = false;

	float xdir = 0;
	float ydir = 0;
	float moveDuration = 0.7;
	float moveTimer = 0;
	bool bAtTarget = false;
};

