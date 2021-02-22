#include "GameObject.h"

GameObject::GameObject() {


	shaderHover.loadFromFile("Textures/Shaders/hover.vert", Shader::Vertex);
}

void GameObject::Draw(RenderWindow* w) {
	if (bHover) {
		w->draw(icon, &shaderHover);
	}
	else {
		w->draw(icon);
	}
}

void GameObject::Update(Time t) {
	if (bMoving) {

		icon.move(Vector2f(xdir, ydir) * t.asSeconds());
		SetPosition(icon.getPosition());
		if (abs(pos.x - targetPos.x) <= 1 && abs(pos.y - targetPos.y) <= 1) {
			bMoving = false;
			SetPosition(targetPos);
			bAtTarget = true;
		}
	}
}

