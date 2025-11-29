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

//ctor
GameEngine::GameEngine(int level1, const string& seq1, int level2, const string& seq2)
    : currPlayer{0}, ci{}, gameOver{false} {
    players.emplace_back(level1, seq1);
    players.emplace_back(level2, seq2);
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

//most basic method, does a single cmd at a time
void GameEngine::executeSingleCommand(const string& cmd) {
    Player& p = currentPlayer();
    bool droppedByCommand = false;

    // ---------------- Basic actions ----------------
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
        droppedByCommand = true;

        // show board before prompting
        notifyObservers();

        if (p.getGameOver()) {
            gameOver = true;
            return;
        }

        // ✅ PROMPT immediately if a special action was earned
        // ✅ PROMPT immediately if a special action was earned
        if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
            cout << "\n🎉 You cleared multiple rows! Choose a special action:" << endl;
            cout << "   heavy" << endl;
            cout << "   blind" << endl;
            cout << "   force <I/J/L/O/S/T/Z>" << endl;
            cout << "Enter your choice: ";
            cout.flush();

            // ✅ simple input flush without numeric_limits
            cin.clear();
            while (cin.peek() == '\n') {
                cin.get(); // consume stray newline(s)
            }

            string choice;
            getline(cin, choice);

            if (choice == "heavy") {
                Heavy effect;
                otherPlayer().applyEffect(&effect);
                p.useOneSpecialAction();
            }
            else if (choice == "blind") {
                Blind effect;
                otherPlayer().applyEffect(&effect);
                p.useOneSpecialAction();
            }
            else if (choice.rfind("force", 0) == 0) {
                istringstream iss(choice);
                string word, block;
                iss >> word >> block;
                if (!block.empty()) {
                    char type = toupper(block[0]);
                    otherPlayer().forceNextBlock(type);
                    p.useOneSpecialAction();
                } else {
                    cout << "Invalid format. Use: force I" << endl;
                }
            } else {
                cout << "Invalid choice, skipping special action." << endl;
            }

            notifyObservers(); // update after effect
        }

    }
    else if (cmd == "hold") {
        p.holdBlock();
    }
    else if (cmd == "levelup") {
        p.incLevel();
    }
    else if (cmd == "leveldown") {
        p.decLevel();
    }
    else if (cmd == "random") {
        p.setRandomMode(true);
    }
    else if (cmd == "restart") {
        for (auto &pl : players) pl.reset();
        gameOver = false;
        currPlayer = 0;
    }
    else if (cmd == "blind") {
        if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
            Blind effect;
            otherPlayer().applyEffect(&effect);
            p.useOneSpecialAction();
        }
    }
    else if (cmd == "heavy") {
        if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
            Heavy effect;
            otherPlayer().applyEffect(&effect);
            p.useOneSpecialAction();
        }
    }
    else if (cmd == "I" || cmd == "J" || cmd == "L" ||
             cmd == "O" || cmd == "S" || cmd == "Z" || cmd == "T") {
        if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
            otherPlayer().forceNextBlock(cmd[0]);
            p.useOneSpecialAction();
        }
    }
    else if (cmd == "quit") {
        gameOver = true;
        return;
    }

    // ---------------- Turn + observer handling ----------------
    if (gameOver) {
        notifyObservers();
        return;
    }

    bool heavyAutoDrop = p.hasJustDropped();
    if (heavyAutoDrop) {
        p.clearJustDropped();
    }

    if (droppedByCommand || heavyAutoDrop) {
        switchTurns();
        notifyObservers();
    }

    if (!gameOver) {
        Player &next = currentPlayer();
        if (next.hasJustDropped()) {
            next.clearJustDropped();
            switchTurns();
        }
    }

    notifyObservers();
}


//read from file to get Block sequence
void GameEngine::executeSequenceFile(const string& filename) {
    ifstream in{filename};
    if (!in) {
        cerr << "Cannot open sequence file: " << filename << endl;
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

    notifyObservers();

    string line;
    while (!gameOver && getline(cin, line)) {
        handleCommand(line);
    }

    end();
}

void GameEngine::end() {
    cout << "Game over" << endl;
    cout << "Final Score P1: " << players[0].getScore() << endl;
    cout << "Final Score P2: " << players[1].getScore() << endl;
}


// takes cmd, expands it via CommandInterpreter, and xecutes each part of the cmd using executeSingleCommand
void GameEngine::handleCommand(const string& cmd) {
    if (gameOver) return;

    string expanded = ci.parse(cmd);
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
            string filename;
            Player& p = currentPlayer();

            if (iss >> filename) {
                Level* logic = p.getLevelLogic();
                if (logic) {
                    logic->readFile(filename);
                    logic->setRandom(false);
                }
            } else {
                p.setRandomMode(false);
            }

            notifyObservers();
        }
        else {
            executeSingleCommand(token);
        }
    }
}
