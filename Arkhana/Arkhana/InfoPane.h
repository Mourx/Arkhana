#pragma once
#include "GameObject.h"
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
    void SetCardInfo(String desc,String name) {
        cardInfo = desc;
        cardInfoName = name;
        txtCardInfo.setString(cardInfo);
        txtCardInfoName.setString(cardInfoName);
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

    Vector2f screenTitlePos = pos + Vector2f(15, 50);
    Vector2f descriptionPos = pos + Vector2f(15,100);
    Vector2f hoverTitlePos = pos + Vector2f(15, 450);
    Vector2f hoverDescriptionPos = pos + Vector2f(15, 500);
    Vector2f cardInfoTitlePos = pos + Vector2f(15, 630);
    Vector2f cardInfoNamePos = pos + Vector2f(15, 660);
    Vector2f cardInfoPos = pos + Vector2f(15, 690);
    

};

