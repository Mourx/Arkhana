#include "InfoPane.h"


InfoPane::InfoPane() {

	
	texIcon.loadFromFile("Textures/GUI/InfoPane.png");
	icon.setTexture(texIcon);
	icon.setPosition(pos);

	font.loadFromFile("Fonts/ManaSpace/manaspc.ttf");
	txtDescription.setFont(font);
	txtDescription.setCharacterSize(18);
	txtDescription.setString("Wow what an enemy! \nSometimes they are in \ngroups of 5!");
	txtDescription.setPosition(descriptionPos);

	txtHoverDescription.setFont(font);
	txtHoverDescription.setCharacterSize(18);
	txtHoverDescription.setString("A fight with foes \nof the castle!");
	txtHoverDescription.setPosition(hoverDescriptionPos);

	txtScreenTitle.setFont(font);
	txtScreenTitle.setCharacterSize(26);
	txtScreenTitle.setStyle(Text::Bold);
	txtScreenTitle.setPosition(screenTitlePos);
	
	txtHoverTitle.setFont(font);
	txtHoverTitle.setCharacterSize(26);
	txtScreenTitle.setStyle(Text::Bold);
	txtHoverTitle.setPosition(hoverTitlePos);
}

void InfoPane::Draw(RenderWindow* w) {
	w->draw(icon);
	w->draw(txtDescription);
	w->draw(txtHoverDescription);
	w->draw(txtHoverTitle);
	w->draw(txtScreenTitle);
}