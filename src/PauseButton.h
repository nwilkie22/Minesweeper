#include <iostream>
#include <vector>
using namespace std;


class PauseButton{
    Textures &textureMap;
    sf::Sprite pauseButton;
public:
    PauseButton(Textures &TEXTUREMAP) : textureMap(TEXTUREMAP) {
        pauseButton.setTexture(textureMap.getTexture("pause"));
    }

    void changePosition(float num_col, float num_row) {
        float width = (num_col  * 32) - 240;
        float height = (num_row + 0.5f) * 32;
        pauseButton.setPosition(width, height);
    }

    bool pauseButtonUpdate(int mouseX, int mouseY){
        if (mouseX >= pauseButton.getPosition().x &&
            mouseX <= pauseButton.getPosition().x + 64 &&
            mouseY >= pauseButton.getPosition().y &&
            mouseY <= pauseButton.getPosition().y + 64){
            return true;
        }
        return false;
    }

    sf::Sprite getSprite(){
        return pauseButton;
    }

    void changeTexture(sf::Texture& texture){
        pauseButton.setTexture(texture);
    }

};