#include <iostream>
#include <vector>
using namespace std;

class FaceButton{
    Textures &textureMap;
    sf::Sprite faceButton;
public:
    FaceButton(Textures& TEXTUREMAP) : textureMap(TEXTUREMAP){
        faceButton.setTexture(textureMap.getTexture("face_happy"));
    }

    void changePosition(float num_col, float num_row){
        float width = ((num_col/2)*32)-32;
        float height = (num_row + 0.5f) * 32;
        faceButton.setPosition(width, height);
    }
    bool faceButtonUpdate(int mouseX, int mouseY){
        if (mouseX >= faceButton.getPosition().x &&
            mouseX <= faceButton.getPosition().x + 64 &&
            mouseY >= faceButton.getPosition().y &&
            mouseY <= faceButton.getPosition().y + 64){
            return true;
        }
        return false;
    }

    sf::Sprite getSprite(){
        return faceButton;
    }

    void changeTexture(sf::Texture& texture){
        faceButton.setTexture(texture);
    }
};