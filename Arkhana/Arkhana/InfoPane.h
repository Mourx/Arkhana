#pragma once
#include "GameObject.h"
class InfoPane :
    public GameObject
{
public:
    InfoPane();
    ~InfoPane() {}
    void Draw(RenderWindow* w);
protected:
    
    Vector2f pos = Vector2f(1600, 180);

    Font font;
    String description;
    Text txtDescription;
    Vector2f descriptionPos = pos + Vector2f(30,100);
};

