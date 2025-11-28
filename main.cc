import GameEngine;
import Player;
import TextDisplay;
import GraphicsDisplay;

import <iostream>;
import <string>;
import <cstdlib>;
import <ctime>;

using namespace std;

int main(int argc, char* argv[]) {
    // default values
    bool textOnly = false;
    int startLevel = 0;
    string scriptFile1 = "sequence1.txt";
    string scriptFile2 = "sequence2.txt";
    int seed = time(nullptr);
    
    // parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-text") {
            textOnly = true;
        } 
        else if (arg == "-seed" && i + 1 < argc) {
            seed = stoi(argv[++i]);
        } 
        else if (arg == "-scriptfile1" && i + 1 < argc) {
            scriptFile1 = argv[++i];
        } 
        else if (arg == "-scriptfile2" && i + 1 < argc) {
            scriptFile2 = argv[++i];
        } 
        else if (arg == "-startlevel" && i + 1 < argc) {
            startLevel = stoi(argv[++i]);
            if (startLevel < 0) startLevel = 0;
            if (startLevel > 4) startLevel = 4;
        }
    }
    
    // seed random number generator
    srand(seed);
    
    // create players 
    Player p1{startLevel, scriptFile1};
    Player p2{startLevel, scriptFile2};
    
    // create game (Subject)
    GameEngine game(startLevel, scriptFile1, startLevel, scriptFile2);
    
    // create displays (Observers)
    TextDisplay textDisplay(&game);
    GraphicsDisplay graphicsDisplay(&game, textOnly);
    
    // OBSERVER PATTERN: attach observers to subject
    game.attach(&textDisplay);
    game.attach(&graphicsDisplay);
    
    // start game :>
    game.start();
    
    return 0;
}