import GameEngine;
import Player;
import TextDisplay;
import GraphicsDisplay;
import Level0;
import Level1;
import Level2;
import Level3;
import Level4;

import <iostream>;
import <string>;
import <cstdlib>;
import <ctime>;

using namespace std;

int main(int argc, char* argv[]) {
    // Default values
    bool textOnly = false;
    int startLevel = 0;
    string scriptFile1 = "sequence1.txt";
    string scriptFile2 = "sequence2.txt";
    int seed = time(nullptr);
    
    // Parse command-line arguments
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
    
    // Seed random number generator
    srand(seed);
    
    // Create initial levels based on startLevel
    Level* level1 = nullptr;
    Level* level2 = nullptr;
    
    switch (startLevel) {
        case 0:
            level1 = make_unique<Level0>(scriptFile1); 
            level2 = make_unique<Level0>(scriptFile2); 
            break;
        case 1:
            level1 = make_unique<Level1>(); 
            level2 = make_unique<Level1>(); 
            break;
        case 2:
            level1 = make_unique<Level2>(); 
            level2 = make_unique<Level2>(); 
            break;
        case 3:
            level1 = make_unique<Level3>(); 
            level2 = make_unique<Level3>(); 
            break;
        case 4:
            level1 = make_unique<Level4>(); 
            level2 = make_unique<Level4>(); 
            break;
    }
    
    // Create players
    Player p1{move(level1), startLevel};
    Player p2{move(level2), startLevel};
    
    // Create game (Subject)
    GameEngine game(move(p1), move(p2));
    
    // Create displays (Observers)
    TextDisplay textDisplay(&game);
    GraphicsDisplay graphicsDisplay(&game, textOnly);
    
    // OBSERVER PATTERN: Attach observers to subject
    game.attach(&textDisplay);
    game.attach(&graphicsDisplay);
    
    // Start game - CommandInterpreter is used INSIDE game.start()
    game.start();
    
    return 0;
}
