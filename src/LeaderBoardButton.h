#include <iostream>
#include <vector>
using namespace std;

class LeaderBoardButton{
    Textures &textureMap;
    sf::Sprite lbButton;
public:
    LeaderBoardButton(Textures& TEXTUREMAP) : textureMap(TEXTUREMAP){
        lbButton.setTexture(textureMap.getTexture("leaderboard"));
    }

    void changePosition(float num_col, float num_row) {
        float width = (num_col  * 32) - 176;
        float height = (num_row + 0.5f) * 32;
        lbButton.setPosition(width, height);
    }

    bool LBButtonUpdate(int mouseX, int mouseY){
        if (mouseX >= lbButton.getPosition().x &&
            mouseX <= lbButton.getPosition().x + 64 &&
            mouseY >= lbButton.getPosition().y &&
            mouseY <= lbButton.getPosition().y + 64){
            return true;
        }
        return false;
    }

    sf::Sprite getSprite(){
        return lbButton;
    }

    void changeTexture(sf::Texture& texture){
        lbButton.setTexture(texture);
    }


};