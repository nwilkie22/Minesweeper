#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Textures.h"
using namespace std;

class Tile{
    int posX;
    int posY;
    int colNum;
    int rowNum;
    int number;
    sf::Sprite tile;
    sf::Sprite flagSprite;
    Textures &textureMap;
    bool revealed = false;
    bool hasMine = false;
    bool flagged = false;
    bool drawFlag = true;
    vector<Tile*> adjacentTiles;
public:
    Tile(Textures& TEXTUREMAP, int X, int Y, int COLNUM, int ROWNUM) : textureMap(TEXTUREMAP){
        posX = X;
        posY = Y;
        colNum = COLNUM;
        rowNum = ROWNUM;
        flagSprite.setTexture(textureMap.getTexture("flag"));
    }

    bool operator==(const Tile& other) const{
        return posX == other.posX && posY == other.posY;
    }

    bool operator!=(const Tile& other) const{
        return !operator==(other);
    }

    void changeTexture(sf::Texture& texture){
        tile.setTexture(texture);
    }
    void changePosition(float x, float y){
        tile.setPosition(x,y);
        flagSprite.setPosition(x,y);
    }

    bool Update(bool revealMines){
        if(!revealed && !flagged) {
            if (!hasMine) {
                if (number == 0) {
                    tile.setTexture(textureMap.getTexture("tile_revealed"));
                    revealed = true;
                    /*
                    cout << "Tile Revealed: " << rowNum << " ," << colNum << endl;
                    for (const auto &n: adjacentTiles) {
                        cout << "(" << rowNum << " ," << colNum << ") ";
                    }
                    cout << endl;
                     */
                    for (int i = 0; i < adjacentTiles.size(); i++) {
                        /*
                        cout << "Tile " << rowNum << ", " << colNum;
                        cout << " accessed: " << adjacentTiles[i]->rowNum << ", " << adjacentTiles[i]->colNum << endl;
                        */
                        adjacentTiles[i]->Update(false);
                    }

                    return true;
                }
                if (number > 0) {
                    string filename = "number_" + to_string(number);
                    //cout << filename << endl;
                    tile.setTexture(textureMap.getTexture(filename));
                    revealed = true;
                    return true;
                }
            } else {
                if (revealMines) {
                    tile.setTexture(textureMap.getTexture("mine"));
                    revealed = true;
                    return false;
                }
            }
        }
        else{
            return true;
        }
    }

    void flag(){
        if(!revealed){
            if(!flagged){
                flagged = true;
            }
            else{

                flagged = false;
            }
        }
    }

    void DrawFlag(){
        if (drawFlag){
            drawFlag = false;
        }
        else{
            drawFlag = true;
        }
    }

    void addAdjacentTile(Tile* adjacent){
        adjacentTiles.push_back(adjacent);
    }
    void addMine(){
        hasMine = true;
    }

    void assignNumber(){
        int count = 0;
        //cout << adjacentTiles.size() << endl;
        for(int i = 0; i < adjacentTiles.size(); i++){
            if (adjacentTiles[i]->hasMine){
                count += 1;
            }
        }
        number = count;

    }

    int getX(){
        return posX;
    }
    int getY(){
        return posY;
    }
    int getColNum(){
        return colNum;
    }
    int getRowNum(){
        return rowNum;
    }

    int getNumber(){
        return number;
    }

    bool HasMine(){
        return hasMine;
    }

    bool isFlagged(){
        return flagged;
    }

    bool isRevealed(){
        return revealed;
    }

    bool canDrawFlag(){
        return drawFlag;
    }

    sf::Sprite getTile(){
        return tile;
    }

    sf::Sprite getFlag(){
        return flagSprite;
    }
};