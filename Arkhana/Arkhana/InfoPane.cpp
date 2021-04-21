#include "InfoPane.h"


InfoPane::InfoPane() {

	
	texIcon.loadFromFile("Textures/GUI/InfoPane.png");
	icon.setTexture(texIcon);
	icon.setPosition(pos);

	font.loadFromFile("Fonts/ManaSpace/manaspc.ttf");
	txtDescription.setFont(font);
	txtDescription.setCharacterSize(22);
	txtDescription.setString("Wow what an enemy! \nSometimes they are in \ngroups of 5!");
	txtDescription.setPosition(descriptionPos);
}

void InfoPane::Draw(RenderWindow* w) {
	w->draw(icon);
	w->draw(txtDescription);
}