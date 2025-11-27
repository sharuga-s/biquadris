module GameEngine;

import Player;
import Subject;
import CommandInterpreter;

import <iostream>;
import <fstream>;
import <sstream>;
import <string>;
import <utility>;
import <vector>;

using namespace std;

GameEngine::GameEngine(Player p1, Player p2)
    : currPlayer{0}, ci{}, gameOver{false} {
        players.push_back(move(p1));
        players.push_back(move(p2));
}

Player& GameEngine::currentPlayer() {
    return players[currPlayer];
}

Player& GameEngine::otherPlayer() {
    return players[1 - currPlayer];
}

void GameEngine::switchTurns() {
    currPlayer = 1 - currPlayer;
}

void GameEngine::executeSingleCommand(const string& cmd) {
    Player& p = currentPlayer();

    // 1. player movement
    if (cmd == "left") {
        p.moveBlockLeft();
    }
    else if (cmd == "right") {
        p.moveBlockRight();
    }
    else if (cmd == "down") {
        p.moveBlockDown();
    }
    else if (cmd == "clockwise") {
        p.rotateCW();
    }
    else if (cmd == "counterclockwise") {
        p.rotateCCW();
    }
    else if (cmd == "drop") {
        p.dropBlock();

        if (p.isGameOver()) {
            gameOver = true;
        } else {
            switchTurns();
        }
    } else if (cmd == "hold") {
        p.holdBlock();
    } else if (cmd == "levelup") {
        p.incLevel();
    } else if (cmd == "leveldown") {
        p.decLevel();
    } else if (cmd == "random") {
        p.setRandomMode(true);
    } else if (cmd == "norandom") {
        p.setRandomMode(false);
    } else if (cmd == "I" || cmd == "J" || cmd == "L" ||
               cmd == "O" || cmd == "S" || cmd == "Z" || cmd == "T") { 
        // SPECIAL ACTION: FORCE NEXT BLOCK on the *other* player
        otherPlayer().forceNextBlock(cmd[0]);
    }

    // OBSERVER PATTERN: Notify all observers after state change!
    notifyObservers();
}

void GameEngine::executeSequenceFile(const string& filename) {
    ifstream in{filename};
    if (!in) {
        cerr << "Cannot open sequence file: " << filename << '\n';
        return;
    }

    string line;
    while (getline(in, line) && !gameOver) {
        handleCommand(line);
    }
}

void GameEngine::start() {
    gameOver = false;
    currPlayer = 0;

    // OBSERVER PATTERN: Initial notification
    notifyObservers();

    string line;
    while (!gameOver && getline(cin, line)) {
        handleCommand(line);
    }

    end();
}

void GameEngine::end() {
    cout << "Game over.\n";
    cout << "Final Score P1: " << players[0].getScore() << '\n';
    cout << "Final Score P2: " << players[1].getScore() << '\n';
}

void GameEngine::handleCommand(const string& rawCmd) {
    if (gameOver) return;

    string expanded = ci.parse(rawCmd);
    if (expanded.empty()) return;

    istringstream iss{expanded};
    string token;

    while (iss >> token && !gameOver) {
        if (token == "sequence") {
            string filename;
            if (!(iss >> filename)) break;
            executeSequenceFile(filename);
        } else if (token == "norandom") {
            string filename;
            if (!(iss >> filename)) {
                cerr << "norandom requires a filename\n";
                break;
            }
            
            Player& p = currentPlayer();
            
            // If player is in level 3 or 4, set the sequence file
            if (p.getLevel() == 3) {
                Level3* lvl3 = dynamic_cast<Level3*>(p.getLevelLogic());
                if (lvl3) {
                    lvl3->setSequenceFile(filename);
                    lvl3->setRandom(false);
                }
            } else if (p.getLevel() == 4) {
                Level4* lvl4 = dynamic_cast<Level4*>(p.getLevelLogic());
                if (lvl4) {
                    lvl4->setSequenceFile(filename);
                    lvl4->setRandom(false);
                }
            }
            updateDisplays();
        } else {
            executeSingleCommand(token);
        }
    }
}
