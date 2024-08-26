#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

class Leaderboard{
    sf::RenderWindow& lbWindow;
    sf::Text leaderboard;
    sf::Text lbContent;
    sf::Font font;
    sf::Clock timer;
    float width;
    float height;
    string winnerName = "";
    int winnerTime = 0;

public:
    Leaderboard(float WIDTH, float HEIGHT, sf::RenderWindow &LBWINDOW, sf::Clock& TIMER) : lbWindow(LBWINDOW), timer(TIMER) {
        if (!font.loadFromFile("font.ttf")) {
            cout << "Error loading font" << endl;
        }

        width = WIDTH;
        height = HEIGHT;

        //leaderboard text
        leaderboard.setFont(font);
        leaderboard.setString("LEADERBOARD");
        leaderboard.setCharacterSize(20);
        leaderboard.setFillColor(sf::Color::White);
        leaderboard.setStyle(sf::Text::Bold | sf::Text::Underlined);
        setText(leaderboard, WIDTH/2, HEIGHT / 2 - 120);
        //leaderboard.setPosition(0,0);

        //lb content
        lbContent.setFont(font);
        lbContent.setString("content");
        lbContent.setCharacterSize(18);
        lbContent.setFillColor(sf::Color::White);
        lbContent.setStyle(sf::Text::Bold);
        setText(lbContent, width/2, height / 2 + 20);
    }

    void UpdateLBContent(){
        // retrieve info from CSV
        string newContent;
        ifstream file("leaderboard.txt");
        string line;
        vector<string> components;
        while(getline(file, line)){
            istringstream stream(line);
            string component;
            while(getline(stream, component, ',')){
                components.push_back(component);
            }
        }

        // writing newContent
        int tempNum = 1;
        for(int i = 0; i < components.size(); i++){
            if(i % 2 == 0 || i == 0){
                // set up time variables
                int minutes =  stoi(components[i + 1]) / 60;
                int seconds =  stoi(components[i + 1]) % 60;
                string minStr = to_string(minutes);
                string secStr = to_string(seconds);
                if(minutes < 10){
                    minStr = "0" + minStr;
                }
                if(seconds < 10){
                    secStr = "0" + secStr;
                }

                newContent += to_string(tempNum);
                newContent += ".\t";
                newContent += minStr;
                newContent += ":";
                newContent += secStr;
                newContent += "\t";
                newContent += components[i];
                if(components[i] == winnerName && stoi(components[i+1]) == winnerTime){
                    newContent += "*";
                }
                newContent += "\n\n";
                tempNum += 1;
            }
        }

        lbContent.setString(newContent);
        setText(lbContent, width/2, height / 2 + 20);

    }

    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    void DrawBoard(){
        lbWindow.draw(leaderboard);
        lbWindow.draw(lbContent);
    }

    void getWinnersInfo(string wName, int wNum){
        winnerName = wName;
        winnerTime = wNum;
    }
};