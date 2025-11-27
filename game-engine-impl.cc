module GameEngine;

import Player;
import Subject;
import CommandInterpreter;
import Blind;
import Heavy;
import Level;

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
    } else if (cmd == "blind") {
        // apply Blind to the *other* player
        Blind effect;
        otherPlayer().applyEffect(&effect);
    }
    else if (cmd == "heavy") {
        // apply Heavy to the *other* player
        Heavy effect;
        otherPlayer().applyEffect(&effect);
    }
    else if (cmd == "I" || cmd == "J" || cmd == "L" ||
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
        }
        else if (token == "norandom") {
            // Support "norandom <file>" for levels 3 & 4 (and any Level that
            // implements readFile + setRandom).
            string filename;
            if (iss >> filename) {
                Player& p = currentPlayer();
                Level* logic = p.getLevelLogic();
                if (logic) {
                    logic->readFile(filename);   // load sequence
                    logic->setRandom(false);     // disable random
                }
                // state changed → notify
                notifyObservers();
            } else {
                // just "norandom" → delegate to normal handling
                executeSingleCommand("norandom");
            }
        }
        else {
            executeSingleCommand(token);
        }
    }
}
