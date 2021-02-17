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

