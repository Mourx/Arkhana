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

	txtCardInfo.setFont(font);
	txtCardInfo.setCharacterSize(18);
	txtCardInfo.setPosition(cardInfoPos);

	txtCardInfoName.setFont(font);
	txtCardInfoName.setCharacterSize(18);
	txtCardInfoName.setPosition(cardInfoNamePos);

	txtCardInfoTitle.setFont(font);
	txtCardInfoTitle.setCharacterSize(26);
	txtCardInfoTitle.setString(cardInfoTitle);
	txtCardInfoTitle.setStyle(Text::Bold);
	txtCardInfoTitle.setPosition(cardInfoTitlePos);
}

void InfoPane::Draw(RenderTexture* w) {
	w->draw(icon);
	w->draw(txtDescription);
	w->draw(txtHoverDescription);
	w->draw(txtHoverTitle);
	w->draw(txtScreenTitle);
	w->draw(txtCardInfo);
	w->draw(txtCardInfoTitle);
	w->draw(txtCardInfoName);
}