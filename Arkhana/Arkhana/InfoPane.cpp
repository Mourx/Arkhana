#include "InfoPane.h"
#include "DataBase.h"


InfoPane::InfoPane() {

	
	texIcon.loadFromFile("Textures/GUI/InfoPane.png");
	icon.setTexture(texIcon);
	icon.setPosition(pos);

	Font* font = &database->coolFont;
	txtDescription.setFont(*font);
	txtDescription.setCharacterSize(18);
	txtDescription.setString("Welcome to Forty Frogs!\nClick on a Frog to begin!\nEach frog starts with\nits own deck!");
	txtDescription.setPosition(descriptionPos);

	txtHoverDescription.setFont(*font);
	txtHoverDescription.setCharacterSize(18);
	txtHoverDescription.setString("A fight with foes \nof the castle!");
	txtHoverDescription.setPosition(hoverDescriptionPos);

	txtScreenTitle.setFont(*font);
	txtScreenTitle.setCharacterSize(26);
	txtScreenTitle.setStyle(Text::Bold);
	txtScreenTitle.setPosition(screenTitlePos);
	
	txtHoverTitle.setFont(*font);
	txtHoverTitle.setCharacterSize(26);
	txtScreenTitle.setStyle(Text::Bold);
	txtHoverTitle.setPosition(hoverTitlePos);

	txtCardInfo.setFont(*font);
	txtCardInfo.setCharacterSize(18);
	txtCardInfo.setPosition(cardInfoPos);

	txtCardInfoName.setFont(*font);
	txtCardInfoName.setCharacterSize(18);
	txtCardInfoName.setPosition(cardInfoNamePos);


	txtUnitPower.setFont(*font);
	txtUnitPower.setCharacterSize(18);
	txtUnitPower.setPosition(unitPowerPos);

	txtUnitStamina.setFont(*font);
	txtUnitStamina.setCharacterSize(18);
	txtUnitStamina.setPosition(unitStaminaPos);

	txtUnitMods.setFont(*font);
	txtUnitMods.setCharacterSize(18);
	txtUnitMods.setPosition(unitModsPos);

	txtCardInfoTitle.setFont(*font);
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
	w->draw(txtUnitPower);
	w->draw(txtUnitStamina);
	w->draw(txtUnitMods);
}