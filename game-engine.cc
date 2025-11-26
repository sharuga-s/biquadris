export module GameEngine;

import Player;
import TextDisplay;      
import GraphicDisplay; 
import CommandInterpreter;

import <vector>;
import <string>;

using namespace std;

/*
1. currPlayer : int
2. vector<Player, 2> 
3. ci : CommandInterpreter
4. textDisplay : Display
5. graphicsDisplay : Display
6. bool gameOver;
7. void switchTurns();
8. void updateDisplays()

relationships
- OWNS-A [composition] Player
- OWNS-A [composition] Display
- HAS-A [aggregation] CommandInterpreter
*/

export class GameEngine {
    private:
        // fields
        int currPlayer = 0;
        vector<Player> players; // 2 players per ge
        CommandInterpreter ci;
        TextDisplay textDisplay;
        GraphicsDisplay graphicsDisplay;
        bool gameOver = false;

        // helpers
        Player& currentPlayer();
        Player& otherPlayer();
        void switchTurns();
        void updateDisplays();
        void executeSingleCommand(const string& cmd); // single fr command, ie. "left", "drop", "levelup"
        void executeSequenceFile(const string& filename); // helper for "sequence <file>"
    public:
        GameEngine(Player p1, Player p2, TextDisplay td, GraphicsDisplay gd);
        void start(); // read commands from std::cin and play until game over
        void end(); // end current game 
        void handleCommand(string cmd); // execute a raw line of input (used by start and sequence files)
};