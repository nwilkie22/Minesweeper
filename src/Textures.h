#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
using namespace std;

class Textures{
    map<string, sf::Texture> TextureList;
    vector<string> TextureFileNames;

public:
    Textures(){
        TextureFileNames.push_back("images/debug.png");
        TextureFileNames.push_back("images/digits.png");
        TextureFileNames.push_back("images/face_happy.png");
        TextureFileNames.push_back("images/face_lose.png");
        TextureFileNames.push_back("images/face_win.png");
        TextureFileNames.push_back("images/flag.png");
        TextureFileNames.push_back("images/leaderboard.png");
        TextureFileNames.push_back("images/mine.png");
        TextureFileNames.push_back("images/number_1.png");
        TextureFileNames.push_back("images/number_2.png");
        TextureFileNames.push_back("images/number_3.png");
        TextureFileNames.push_back("images/number_4.png");
        TextureFileNames.push_back("images/number_5.png");
        TextureFileNames.push_back("images/number_6.png");
        TextureFileNames.push_back("images/number_7.png");
        TextureFileNames.push_back("images/number_8.png");
        TextureFileNames.push_back("images/pause.png");
        TextureFileNames.push_back("images/play.png");
        TextureFileNames.push_back("images/tile_hidden.png");
        TextureFileNames.push_back("images/tile_revealed.png");
        loadTextures();
    }


    // Do not call this manually!
    void loadTextures(){
        for (int i = 0; i < TextureFileNames.size(); i++){
            sf::Texture texture;
            if (!texture.loadFromFile(TextureFileNames[i])) {
                cout << "Could not load texture!" << endl;
            }
            else{
                cout << "Texture loaded: " << TextureFileNames[i] << endl;
            }
            TextureList.emplace(TextureFileNames[i], texture);

        }
    }


    // to get the texture, use the texture name as a parameter. Do not include images/ or .png
    sf::Texture& getTexture(const string &textureName){
        string textureNameConverted = "images/" + textureName + ".png";
        for(auto it = TextureList.begin(); it != TextureList.end(); it++) {
            if (it->first == textureNameConverted) {
                return it->second;
            }
        }
        throw runtime_error("Texture not found: " + textureNameConverted);
    }

};