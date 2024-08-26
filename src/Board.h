#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <random>
#include "Tile.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include "FaceButton.h"
#include "PauseButton.h"
#include "Debug.h"
#include "LeaderBoardButton.h"
#include "Numbers.h"
using namespace std;


class Board{
    sf::RenderWindow& gamewindow;
    Textures &textureMap;
    string username;
    float height;
    float width;
    int col_num;
    int row_num;
    int mine_num;
    int tile_count;
    int num_flags = 0;
    bool game_Running;
    bool all_Mines_Are_Revealed;
    bool leaderboard;
    bool game_Over;
    sf::Clock timer;
    float pausedTime = 0;
    float totalPauseTime = 0;
    float time = 0;
    vector<Tile> tileMap;
    FaceButton faceButton;
    Debug debug;
    PauseButton pauseButton;
    LeaderBoardButton lbButton;
    Numbers numbers;
    string winnerName = "";
    int winnerTime = 0;
    // stores the location and the index of the tile
    map<pair<int,int>,int> Locations;

public:
    Board(sf::RenderWindow &GAMEWINDOW, Textures& TEXTUREMAP, float HEIGHT, float WIDTH, int COLUMN, int ROWNUM, int MINECOUNT, sf::Clock& TIMER, string &USERNAME)
        : gamewindow(GAMEWINDOW), textureMap(TEXTUREMAP), height(HEIGHT), width(WIDTH), col_num(COLUMN), row_num(ROWNUM), mine_num(MINECOUNT),
          faceButton(TEXTUREMAP), debug(TEXTUREMAP), pauseButton(TEXTUREMAP), lbButton(TEXTUREMAP), timer(TIMER), numbers(TEXTUREMAP), username(USERNAME){
        tile_count = row_num * col_num;
        game_Running = true;
        game_Over = false;
        faceButton.changePosition(float(col_num), float(row_num));
        debug.changePosition(float(col_num), float(row_num));
        pauseButton.changePosition(float(col_num), float(row_num));
        lbButton.changePosition(float(col_num), float(row_num));
        numbers.changePosition(float(col_num), float(row_num));
    }

    /*
    bool areTilesAdajcent(Tile& tile1, Tile& tile2) {
        int x1 = tile1.getX();
        int y1 = tile1.getY();
        int x2 = tile2.getX();
        int y2 = tile2.getY();
        // if the distance between tiles is <1 than they are adjacent
        int distanceX = abs(x2 - x1);
        int distanceY = abs(y2 - y1);
        // covers every case but dx and dy = 0 in which case they are the same tile
        bool areAdjacent = (distanceX == 1 && distanceY == 0) || (distanceX == 0 && distanceY == 1) || (distanceX == 1 && distanceY == 1);
        cout << areAdjacent << endl;
        return (areAdjacent);
    }
     */

    void generateTileMap() {
        int x = 0;
        int y = 0;

        // This loop creates the tile grid
        for (int i = 0; i < row_num; i++) {
            x = 0;

            for (int j = 0; j < col_num; j++) {
                Tile tile(textureMap, x, y, (x / 32), (y / 32));
                tile.changeTexture(textureMap.getTexture("tile_hidden"));
                tile.changePosition((float) x, (float) y);
                tileMap.push_back(tile);
                Locations.emplace(make_pair(tile.getRowNum(), tile.getColNum()), tileMap.size() - 1);
                x += 32;
            }
            y += 32;
        }

        // This loop assigns mines
        int num;
        vector<int> selectedNums;
        srand(std::time(NULL));
        for (int i = 0; i < mine_num; i++) {
            // unique number is generated
            bool newNumselected = false;
            while (!newNumselected) {
                newNumselected = true;
                num = rand() % tile_count;
                for (int j = 0; j < selectedNums.size(); j++) {
                    if (num == selectedNums[j]) {
                        newNumselected = false;
                        break;
                    }
                }
            }
            // number is added to a vector so that we can ensure it does not get chosen again
            selectedNums.push_back(num);
            // mine is assigned
            tileMap[num].addMine();
        }

        // This loop assigns neighbors
        for (int i = 0; i < tileMap.size(); i++) {

            int row = tileMap[i].getRowNum();
            int col = tileMap[i].getColNum();

            int lowBoundRow = (row - 1);
            int highBoundRow = (row + 1);
            int lowBoundCol = (col - 1);
            int highBoundCol = (col + 1);

            if (lowBoundRow < 0) { lowBoundRow = 0; }
            if (highBoundRow >= row_num) { highBoundRow = row_num - 1; }
            if (lowBoundCol < 0) { lowBoundCol = 0; }
            if (highBoundCol >= col_num) { highBoundCol = col_num - 1; }


            set<pair<int, int>> neighbors;

            for (int r = lowBoundRow; r <= highBoundRow; ++r) {
                for (int c = lowBoundCol; c <= highBoundCol; ++c) {
                    if (!(r == row && c == col)) {
                        neighbors.insert({r, c});
                    }
                }
            }

            // neighbors are stored in pairs <row, col>. The location of each set is stored in a map along with its index in the TileMap
            /*
            for (const auto &n: neighbors) {
                cout << "(" << n.first << ", " << n.second << ") ";
            }
            cout << endl;
             */

            for (const auto &pairs: neighbors) {
                tileMap[i].addAdjacentTile(&tileMap[Locations[pairs]]);
            }


            // This loop assigns numbers to tiles bordering mines that are not mines themselves
            for (int i = 0; i < tileMap.size(); i++) {
                if(!tileMap[i].HasMine())
                tileMap[i].assignNumber();
            }
        }
        numbers.UpdateMineCount(mine_num, num_flags);
    }
    void tileUpdate(int mouseX, int mouseY){
        int TIME = int(timer.getElapsedTime().asSeconds());
        int paramedMouseX = mouseX - (mouseX % 32);
        int paramedMouseY = mouseY - (mouseY % 32);
        for(int i = 0; i < tileMap.size(); i++){
            if (paramedMouseX == tileMap[i].getX() && paramedMouseY == tileMap[i].getY()){
                if(!tileMap[i].Update(true)){
                    gameOver();
                }
            }
        }
        if(debug.debugButtonUpdate(mouseX, mouseY)){
            RevealMines();
        }
        if (faceButton.faceButtonUpdate(mouseX, mouseY)){
            Reset();
        }
        if(lbButton.LBButtonUpdate(mouseX, mouseY)){
            leaderboard = true;
        }
        CheckWin(TIME);
    }

    void tileUpdateOnlyFaceButton(int mouseX, int mouseY){
        if (faceButton.faceButtonUpdate(mouseX, mouseY)){
            Reset();
        }
        if(lbButton.LBButtonUpdate(mouseX, mouseY)){
            leaderboard = true;
        }
    }

    void tileFlag(int mouseX, int mouseY){
        mouseX = mouseX - (mouseX % 32);
        mouseY = mouseY - (mouseY % 32);
        int tempFlagCounter = 0;
        for(int i = 0; i < tileMap.size(); i++){
            if (mouseX == tileMap[i].getX() && mouseY == tileMap[i].getY()){
                tileMap[i].flag();
            }
            if(tileMap[i].isFlagged()){
                tempFlagCounter +=1;
            }
        }
        num_flags = tempFlagCounter;
        //cout << "number of flags: " << num_flags << endl;
        numbers.UpdateMineCount(mine_num, num_flags);
    }

    void gameOver(){
        cout << "Game Over" << endl;
        game_Running = false;
        game_Over = true;
        for (int i = 0; i < tileMap.size(); i++) {
            if(tileMap[i].HasMine()){
                if (tileMap[i].isFlagged()){
                    tileMap[i].flag();
                }
                tileMap[i].changeTexture(textureMap.getTexture("mine"));
            }
        }
        faceButton.changeTexture(textureMap.getTexture("face_lose"));
    }
    void DrawBoard(){
        for (int i = 0; i < tileMap.size(); i++){
            gamewindow.draw(tileMap[i].getTile());
            if (tileMap[i].isFlagged() && tileMap[i].canDrawFlag()){
                gamewindow.draw(tileMap[i].getFlag());
            }
        }
        gamewindow.draw(faceButton.getSprite());
        gamewindow.draw(debug.getSprite());
        gamewindow.draw(pauseButton.getSprite());
        gamewindow.draw(lbButton.getSprite());
        gamewindow.draw(numbers.getNumber1());
        gamewindow.draw(numbers.getNumber2());
        gamewindow.draw(numbers.getNumber3());
        gamewindow.draw(numbers.getNumber4());
        gamewindow.draw(numbers.getNumber5());
        gamewindow.draw(numbers.getNumber6());
        gamewindow.draw(numbers.getNumber7());
    }

    void RevealMines(){
        if(!all_Mines_Are_Revealed){
            for(int i = 0; i < tileMap.size(); i++){
                if(tileMap[i].HasMine()){
                    tileMap[i].changeTexture(textureMap.getTexture("mine"));
                }
                all_Mines_Are_Revealed = true;
            }
        }
        else{
            for(int i = 0; i < tileMap.size(); i++) {
                if(tileMap[i].HasMine()){
                    tileMap[i].changeTexture(textureMap.getTexture("tile_hidden"));
                }
                all_Mines_Are_Revealed = false;
            }
        }
    }

    void CheckWin(int TIME){
        if(!game_Over){
            bool allTilesRevealed = true;
            for(int i = 0; i < tileMap.size(); i++) {
                if (!tileMap[i].isRevealed() && !tileMap[i].HasMine()){
                    allTilesRevealed = false;
                }
            }
            if(allTilesRevealed){
                cout << "Game Won" << endl;
                for(int i = 0; i < tileMap.size(); i++) {
                    if(!tileMap[i].isFlagged() && tileMap[i].HasMine()){
                        tileMap[i].flag();
                    }
                }
                game_Over = true;
                winnerName = username;
                winnerName = username;
                winnerTime = TIME;
                leaderBoardUpdate(TIME, username);
                faceButton.changeTexture(textureMap.getTexture("face_win"));
            }
        }
    }

    void Reset(){
        game_Running = true;
        game_Over = false;
        tileMap.clear();
        generateTileMap();
        all_Mines_Are_Revealed = false;
        timer.restart();
        totalPauseTime = 0;
        string winnerName = "";
        int winnerTime = 0;
        faceButton.changeTexture(textureMap.getTexture("face_happy"));
        pauseButton.changeTexture(textureMap.getTexture("pause"));
    }

    void Pause(int mouseX, int mouseY){
        if(pauseButton.pauseButtonUpdate(mouseX, mouseY)){
            if(!game_Over){
                //pause
                if (game_Running){
                    game_Running = false;
                    pauseButton.changeTexture(textureMap.getTexture("play"));
                    pausedTime = timer.getElapsedTime().asSeconds();
                    for(int i = 0; i < tileMap.size(); i++) {
                        tileMap[i].changeTexture(textureMap.getTexture("tile_revealed"));
                        tileMap[i].DrawFlag();
                    }
                }
                //unpause
                else{
                    game_Running = true;
                    pauseButton.changeTexture(textureMap.getTexture("pause"));
                    totalPauseTime += timer.getElapsedTime().asSeconds() - pausedTime;
                    pausedTime = 0;
                    for(int i = 0; i < tileMap.size(); i++) {
                        if(tileMap[i].isRevealed()){
                            if (tileMap[i].getNumber() > 0) {
                                string filename = "number_" + to_string(tileMap[i].getNumber());
                                //cout << filename << endl;
                                tileMap[i].changeTexture(textureMap.getTexture(filename));
                            }
                            // else is handled bc they will already have the revealed texture
                        }
                        else{
                            tileMap[i].changeTexture(textureMap.getTexture("tile_hidden"));
                        }
                        tileMap[i].DrawFlag();
                    }
                }
            }
        }
    }

    void timeUpdate(){
        if(game_Running && !game_Over){
            time = timer.getElapsedTime().asSeconds() - totalPauseTime;
            numbers.UpdateTimer(int(time));
        }
    }

    float getTime(){
        return time;
    }

    void leaderBoardUpdate(int Time, string Username){
        // retrieve info from CSV
        string newContent;
        ifstream file("leaderboard.txt");
        if(file.is_open()){
            //cout << "file opened" << endl;
        }
        string line;
        vector<string> components;
        while(getline(file, line)){
            istringstream stream(line);
            string component;
            while(getline(stream, component, ',')){
                components.push_back(component);
            }
        }
        file.close();


        vector<string> updatedComponents;

        bool pushedback = false;
        if(components.empty()){
            components.push_back(Username);
            components.push_back(to_string(Time));
        }
        else{
            for(int i = 0; i < components.size(); i += 2) {
                if (Time < stoi(components[i + 1]) && !pushedback) {
                    updatedComponents.push_back(Username);
                    updatedComponents.push_back(to_string(Time));
                    pushedback = true;
                    cout << "pushed" << endl;
                }
                updatedComponents.push_back(components[i]);
                updatedComponents.push_back(components[i + 1]);
                cout << "pushed" << endl;
            }
            if (pushedback && updatedComponents.size() > 10){
                cout << "ran" << endl;
                updatedComponents.pop_back();
                updatedComponents.pop_back();
            }
            components = std::move(updatedComponents);
        }



        ofstream outfile("leaderboard.txt");
        if(outfile.is_open()){
            //cout << "outfile opened" << endl;
        }
        else{
            cout << "ERROR" << endl;
        }
        //cout << "FILE IS UPDATED" << endl;
        for(int i = 0; i < components.size(); i += 2) {
            outfile << components[i] << "," << components[i+1] << endl;
        }
        //cout << "FILE IS CLOSED" << endl;
        outfile.close();
    }

    string getWinnersName(){
        return winnerName;
    }

    int getWinnersTime(){
        return winnerTime;
    }

    void closeLeaderboard(){
        leaderboard = false;
    }
    bool OpenNewWindow(){
        return leaderboard;
    }
    bool IsGameRunning(){
        return game_Running;
    }

};