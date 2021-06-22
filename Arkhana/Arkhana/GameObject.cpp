#include "GameObject.h"

GameObject::GameObject() {


	shaderHover.loadFromFile("Textures/Shaders/hover.vert", Shader::Vertex);
}

void GameObject::Draw(RenderTexture* w) {
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
		if (abs(pos.x - targetPos.x) <= 5 && abs(pos.y - targetPos.y) <= 5) {
			bMoving = false;
			SetPosition(targetPos);
			bAtTarget = true;
		}
	}
}

