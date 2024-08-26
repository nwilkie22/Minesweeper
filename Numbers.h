#include <iostream>
#include <vector>
using namespace std;

class Numbers{
    sf::Sprite number1;
    sf::Sprite number2;
    sf::Sprite number3;
    sf::Sprite number4;
    sf::Sprite number5;
    sf::Sprite number6;
    sf::Sprite number7;
    Textures &textureMap;
    sf::IntRect textureRect;

public:
    Numbers(Textures &TEXTUREMAP) : textureMap(TEXTUREMAP) {
        number1.setTexture(textureMap.getTexture("digits"));
        number2.setTexture(textureMap.getTexture("digits"));
        number3.setTexture(textureMap.getTexture("digits"));
        number4.setTexture(textureMap.getTexture("digits"));
        number5.setTexture(textureMap.getTexture("digits"));
        number6.setTexture(textureMap.getTexture("digits"));
        number7.setTexture(textureMap.getTexture("digits"));

        textureRect.left = 0;
        textureRect.top = 0;
        textureRect.width = 21;
        textureRect.height = 32;

        // timer numbers
        number1.setTextureRect(textureRect);
        number2.setTextureRect(textureRect);
        number3.setTextureRect(textureRect);
        number4.setTextureRect(textureRect);

        // counter numbers
        number5.setTextureRect(textureRect);
        number6.setTextureRect(textureRect);
        number7.setTextureRect(textureRect);
    }

    void changePosition(float num_col, float num_row) {
        float width = (num_col  * 32) - 97;
        float height = 32 * (num_row + 0.5f) + 16;
        number1.setPosition(width, height);
        width += 21;
        number2.setPosition(width, height);
        width += 21;
        number3.setPosition(width, height);
        width += 21;
        number4.setPosition(width, height);


        width = 33;
        height = 32 * (num_row + 0.5f) + 16;
        number5.setPosition(width, height);
        width += 21;
        number6.setPosition(width,height);
        width += 21;
        number7.setPosition(width, height);
    }

    sf::Sprite getNumber1(){
        return number1;
    }

    sf::Sprite getNumber2(){
        return number2;
    }

    sf::Sprite getNumber3(){
        return number3;
    }

    sf::Sprite getNumber4(){
        return number4;
    }

    sf::Sprite getNumber5(){
        return number5;
    }

    sf::Sprite getNumber6(){
        return number6;
    }

    sf::Sprite getNumber7(){
        return number7;
    }

    void UpdateTimer(int totalSeconds){
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        int secondsFirstDigit = seconds % 10;
        int secondsSecondDigit = seconds / 10;
        int minutesFirstDigit = minutes % 10;
        int minutesSecondDigit = minutes / 10;
        ChangeNumber(minutesSecondDigit, number1);
        ChangeNumber(minutesFirstDigit, number2);
        ChangeNumber(secondsSecondDigit, number3);
        ChangeNumber(secondsFirstDigit, number4);
    }

    void UpdateMineCount(int mineCount, int numFlags){
        mineCount = mineCount - numFlags;
        int mineCountDigit0 = mineCount / 100;
        int mineCountFirstDigit = mineCount / 10;
        int mineCountSecondDigit = mineCount % 10;
        cout << "mineCountFirstDigit: " << mineCountFirstDigit << endl;
        cout << "mineCountSecondDigit: " << mineCountSecondDigit << endl;
        cout << "mineCountDigit0" << mineCountDigit0 << endl;
        ChangeNumber(abs(mineCountDigit0), number5);
        ChangeNumber(abs(mineCountFirstDigit), number6);
        ChangeNumber(abs(mineCountSecondDigit), number7);
        if (mineCount < 0){
            ChangeNumber(10, number5);
        }

    }

    void ChangeNumber(int number, sf::Sprite& sprite){
        textureRect.left = 21 * number;
        sprite.setTextureRect(textureRect);
    }

    void reinitializeTextureRect(){
        textureRect.left = 0;
        textureRect.top = 0;
        textureRect.width = 21;
        textureRect.height = 32;
    }
};