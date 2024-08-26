#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <cctype>
#include "Board.h"
#include "Leaderboard.h"
using namespace std;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
    }


int main() {
    // Obtaining col, row, and mine numbers
    fstream file("config.cfg");
    if (file.is_open()){
        cout << "file opened" << endl;
    }
    else{
        cout << "file did not open (sad)" << endl;
    }
    string line;
    getline(file, line);
    int col_num = stoi(line);
    getline(file, line);
    int row_num = stoi(line);
    getline(file, line);
    int mine_num = stoi(line);

    cout << "Col Num: " << col_num << endl;
    cout << "Row Num: " << row_num << endl;
    cout << "Mine Num: " << mine_num << endl;
    //
    //


    // Welcome Window
    sf::RenderWindow welcomeWindow(sf::VideoMode((col_num * 32),(row_num * 32) + 100), "Minesweeper", sf::Style::Close);
    welcomeWindow.setFramerateLimit(60);
    float width = (col_num * 32.0f);
    float height = (row_num * 32.0f) + 100.0f;


    //font
    sf::Font font;
    if(!font.loadFromFile("font.ttf")){
        cout << "Error loading font" << endl;
    }

    // welcome text
    sf::Text welcome;
    welcome.setFont(font);
    welcome.setString("WELCOME TO MINESWEEPER");
    welcome.setCharacterSize(24);
    welcome.setFillColor(sf::Color::White);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcome,width/2,height/2-150);

    // enter name text
    sf::Text enterName;
    enterName.setFont(font);
    enterName.setString("Enter your name:");
    enterName.setCharacterSize(20);
    enterName.setFillColor(sf::Color::White);
    enterName.setStyle(sf::Text::Bold);
    setText(enterName,width/2,height/2-75);

    // player input text
    string input;
    sf::Text userName;
    userName.setFont(font);
    userName.setString("|");
    userName.setCharacterSize(18);
    userName.setFillColor(sf::Color::Yellow);
    userName.setStyle(sf::Text::Bold);
    setText(userName,width/2,height/2-45);


    // WELCOME WINDOW LOOP
    while (welcomeWindow.isOpen()){
        sf::Event event;

        // poll event returns true if an event is pending and false if there was none
        while (welcomeWindow.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    welcomeWindow.close();
                    return 0;

                case sf::Event::Resized:
                    cout << "new width: " << event.size.width << endl;
                    cout << "new height: " << event.size.height << endl;
                    break;

                case sf::Event::TextEntered:
                    if(std::isalpha(char(event.text.unicode)) && input.size() < 10) {
                        input += event.text.unicode;
                        if (input.size() == 1){
                            input[0] = toupper(input[0]);
                        }
                        if (input.size() > 1){
                            input[input.size() - 1] = tolower(input[input.size() - 1]);
                        }
                        userName.setString(input + "|");
                        setText(userName, width / 2, height / 2 - 45);
                    }
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::BackSpace){
                        if (input.size() > 0){
                            input.pop_back();
                            userName.setString(input + "|");
                            setText(userName, width / 2, height / 2 - 45);
                        }
                    }
                    if(event.key.code == sf::Keyboard::Enter){
                        if (input.size() > 0){
                            welcomeWindow.close();
                        }
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Left){
                        cout << "Left Click At: " << "Mouse X: " << event.mouseButton.x << " Mouse Y: " << event.mouseButton.y << endl;
                    }
                    break;

                default:
                    break;
            }
        }
        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcome);
        welcomeWindow.draw(enterName);
        welcomeWindow.draw(userName);
        welcomeWindow.display();
        sf::sleep(sf::milliseconds(16));
    }

    // Game Window
    sf::RenderWindow gameWindow(sf::VideoMode((col_num * 32),(row_num * 32) + 100), "Minesweeper", sf::Style::Close);
    gameWindow.setFramerateLimit(60);

    // Texture Map Loading
    Textures textureMap;

    // Generate Timer
    sf::Clock timer;

    // Generate Board
    Board gameBoard(gameWindow,textureMap,height,width,col_num,row_num,mine_num,timer, input);
    gameBoard.generateTileMap();

    // GAME WINDOW LOOP
    while (gameWindow.isOpen()) {
        sf::Event event;
        gameBoard.timeUpdate();
        // poll event returns true if an event is pending and false if there was none
        while (gameWindow.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    gameWindow.close();
                    return 0;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        cout << "Left Click At: " << "Mouse X: " << event.mouseButton.x << " Mouse Y: " << event.mouseButton.y << endl;
                        gameBoard.Pause(event.mouseButton.x, event.mouseButton.y);
                        if(gameBoard.IsGameRunning()){
                            gameBoard.tileUpdate(event.mouseButton.x, event.mouseButton.y);
                        }
                        else{
                            gameBoard.tileUpdateOnlyFaceButton(event.mouseButton.x, event.mouseButton.y);
                        }
                        if(gameBoard.IsGameRunning()){

                        }
                    }
                    if (event.mouseButton.button == sf::Mouse::Right){
                        cout << "Right Click At: " << "Mouse X: " << event.mouseButton.x << " Mouse Y: " << event.mouseButton.y << endl;
                        if(gameBoard.IsGameRunning()) {
                            gameBoard.tileFlag(event.mouseButton.x, event.mouseButton.y);
                        }
                    }
                    break;

                default:
                    break;
            }

        }
        gameWindow.clear(sf::Color::White);
        gameBoard.DrawBoard();
        gameWindow.display();
        sf::sleep(sf::milliseconds(16));

        if(gameBoard.OpenNewWindow()){
            sf::RenderWindow leaderboardBoard(sf::VideoMode((col_num * 16),(row_num * 16)), "Leaderboard", sf::Style::Close);
            Leaderboard lb((col_num * 16), (row_num * 16), leaderboardBoard, timer);
            lb.getWinnersInfo(gameBoard.getWinnersName(),gameBoard.getWinnersTime());
            lb.UpdateLBContent();
            while (leaderboardBoard.isOpen()) {
                sf::Event newEvent;
                while (leaderboardBoard.pollEvent(newEvent)) {
                    if (newEvent.type == sf::Event::Closed)
                        leaderboardBoard.close();
                        gameBoard.closeLeaderboard();
                }

                leaderboardBoard.clear(sf::Color::Blue);
                lb.DrawBoard();
                leaderboardBoard.display();
                sf::sleep(sf::milliseconds(16));
            }
        }
    }

    return 0;
}