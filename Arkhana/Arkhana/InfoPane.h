#pragma once
#include "GameObject.h"
#include "Card.h"
class InfoPane :
    public GameObject
{
public:
    InfoPane();
    ~InfoPane() {}
    void Draw(RenderTexture* w);
    void SetDescription(String s) {
        description = s;
        txtDescription.setString(description);
    }
    void SetScreenTitle(String s) {
        screenTitle = s;
        txtScreenTitle.setString(screenTitle);
    }
    void SetHoverDescription(String s) {
        hoverDescription = s;
        txtHoverDescription.setString(hoverDescription);
    }
    void SetHoverTitle(String s) {
        hoverTitle = s;
        txtHoverTitle.setString(hoverTitle);
    }
    void SetCardInfo(Card* card) {
        cardInfo = card->GetDesc();
        cardInfoName = card->GetName();
        txtCardInfo.setString(cardInfo);
        txtUnitPower.setString("");
        txtUnitStamina.setString("");
        txtUnitMods.setString("");
        txtCardInfoName.setString(cardInfoName);
        unitMods = "";
    }
    void SetUnitInfo(Unit* unit) {
        cardInfo = "";
        cardInfoName = unit->GetName();
        txtCardInfo.setString(cardInfo);
        txtCardInfoName.setString(cardInfoName);
        if (!unit->IsUndercover() && !unit->IsPassive()) {
            unitPower = "Power:   " + to_string((int)unit->GetPPower());
        }
        else {
            unitPower = "Power:   0";
        }
        txtUnitPower.setString(unitPower);
        unitStamina = "Stamina: " + to_string((int)unit->GetStamina());
        txtUnitStamina.setString(unitStamina);

        unitMods = "";
        for (Modifier* mod : unit->GetModifiers()) {
            unitMods += mod->GetShortText() + "\n";
        }for (Modifier* mod : unit->GetZoneModifiers()) {
            unitMods += mod->GetShortText() + "\n";
        }
        txtUnitMods.setString(unitMods);
    }
protected:
    
    Vector2f pos = Vector2f(1600, 180);

    Font font;
    String description;
    Text txtDescription;

    String hoverDescription;
    Text txtHoverDescription;

    String screenTitle;
    String hoverTitle;

    Text txtScreenTitle;
    Text txtHoverTitle;

    String cardInfo;
    Text txtCardInfo;

    String cardInfoTitle = "CARD / UNIT INFO:";
    Text txtCardInfoTitle;

    String cardInfoName;
    Text txtCardInfoName;

    String unitPower;
    Text txtUnitPower;

    String unitStamina;
    Text txtUnitStamina;

    String unitMods;
    Text txtUnitMods;

    Vector2f screenTitlePos = pos + Vector2f(10, 50);
    Vector2f descriptionPos = pos + Vector2f(10,100);
    Vector2f hoverTitlePos = pos + Vector2f(10, 250);
    Vector2f hoverDescriptionPos = pos + Vector2f(10, 280);
    Vector2f cardInfoTitlePos = pos + Vector2f(10, 400);
    Vector2f cardInfoNamePos = pos + Vector2f(10, 430);
    Vector2f cardInfoPos = pos + Vector2f(10, 505);
    Vector2f unitPowerPos = pos + Vector2f(10, 460);
    Vector2f unitStaminaPos = pos + Vector2f(10, 480);
    Vector2f unitModsPos = pos + Vector2f(10, 505);

};

