#include "GameObject.h"

GameObject::GameObject() {

}

void GameObject::Draw(RenderWindow* w) {
	w->draw(icon);
}

