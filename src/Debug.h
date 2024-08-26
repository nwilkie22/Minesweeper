#include <iostream>
#include <vector>
using namespace std;


class Debug{
    Textures &textureMap;
    sf::Sprite debug;
public:
    Debug(Textures &TEXTUREMAP) : textureMap(TEXTUREMAP) {
        debug.setTexture(textureMap.getTexture("debug"));
    }

    void changePosition(float num_col, float num_row) {
        float width = (num_col  * 32) - 304;
        float height = (num_row + 0.5f) * 32;
        debug.setPosition(width, height);
    }

    bool debugButtonUpdate(int mouseX, int mouseY){
        if (mouseX >= debug.getPosition().x &&
            mouseX <= debug.getPosition().x + 64 &&
            mouseY >= debug.getPosition().y &&
            mouseY <= debug.getPosition().y + 64){
            return true;
        }
        return false;
    }

    sf::Sprite getSprite(){
        return debug;
    }

};