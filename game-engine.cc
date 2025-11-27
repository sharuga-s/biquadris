export module GameEngine;

import Player;
import Subject;  // Import Subject base class
import CommandInterpreter;

import <vector>;
import <string>;

using namespace std;

export class GameEngine : public Subject {  // Inherit from Subject!
    private:
        // fields
        int currPlayer = 0;
        vector<Player> players; // 2 players per ge
        CommandInterpreter ci;
        bool gameOver = false;

        // helpers
        Player& currentPlayer();
        Player& otherPlayer();
        void switchTurns();
        void executeSingleCommand(const string& cmd);
        void executeSequenceFile(const string& filename);
    public:
        GameEngine(Player p1, Player p2);
        void start();
        void end();
        void handleCommand(string cmd);
        
        // Public getters for observers to access state
        const Player& getPlayer1() const { return players[0]; }
        const Player& getPlayer2() const { return players[1]; }
};
