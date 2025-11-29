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

        notifyObservers();
        if (p.getGameOver()) {
            gameOver = true;
            return;
        }

        // prompt for special action after clearing multiple rows
        if (p.hasSpecialAction()) {
            while (true) {
                cout << "\nYou cleared multiple rows! Choose a special action:" << endl;
                cout << "   heavy" << endl;
                cout << "   blind" << endl;
                cout << "   force I/J/L/O/S/T/Z\n";
                cout << "Enter your choice: ";

                string choice;
                if (!getline(cin, choice)) {
                    gameOver = true;
                    return;
                }

                if (choice == "heavy") {
                    Heavy effect; otherPlayer().applyEffect(&effect);
                    p.useOneSpecialAction(); break;
                }
                else if (choice == "blind") {
                    Blind effect; otherPlayer().applyEffect(&effect);
                    p.useOneSpecialAction(); break;
                }
                else {
                    istringstream iss(choice);
                    string word; char typeChar;
                    if (iss >> word >> typeChar && word == "force") {
                        if (typeChar >= 'a' && typeChar <= 'z') typeChar = typeChar - 'a' + 'A';
                        if (typeChar == 'I' || typeChar == 'J' || typeChar == 'L' ||
                            typeChar == 'O' || typeChar == 'S' || typeChar == 'Z' ||
                            typeChar == 'T') {
                            otherPlayer().forceNextBlock(typeChar);
                            p.useOneSpecialAction(); break;
                        }
                    }
                    cout << "Invalid choice. Please enter: heavy, blind, or force I/J/L/O/S/T/Z.\n";
                }
            }
            notifyObservers();
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

    // ===== Special-action restricted commands =====
    else if (cmd == "heavy" || cmd == "blind" || cmd == "force" ||
             cmd == "I" || cmd == "J" || cmd == "L" ||
             cmd == "O" || cmd == "S" || cmd == "Z" || cmd == "T") {
        if (p.hasSpecialAction()) {
            if (cmd == "heavy") {
                Heavy effect; otherPlayer().applyEffect(&effect);
                p.useOneSpecialAction();
            }
            else if (cmd == "blind") {
                Blind effect; otherPlayer().applyEffect(&effect);
                p.useOneSpecialAction();
            }
            else if (cmd == "force") {
                cout << "Usage: force I/J/L/O/S/T/Z\n";
            }
            else { // block letters
                otherPlayer().forceNextBlock(cmd[0]);
                p.useOneSpecialAction();
            }
        } else {
            cout << "You can’t use that command right now — no special action available." << endl;
            executeSingleCommand("");
        }
    }

    else if (cmd == "quit") {
        gameOver = true;
        return;
    }

    // ===== Re-prompt loop for invalid commands =====
    else {
        while (true) {
            cout << "Please enter a valid command: ";

            string line;
            if (!getline(cin, line)) {
                gameOver = true;
                return;
            }

            string expanded = ci.parse(line);

            istringstream iss(expanded);
            string token;
            bool executedSomething = false;

            while (iss >> token && !gameOver) {
                if ((token == "heavy" || token == "blind" || token == "force" ||
                     token == "I" || token == "J" || token == "L" ||
                     token == "O" || token == "S" || token == "Z" || token == "T")
                    && !p.hasSpecialAction()) {
                    cout << "You can’t use that command right now — no special action available.\n";
                    executedSomething = false;
                    continue;
                }
                executeSingleCommand(token);
                executedSomething = true;
            }

            if (executedSomething || gameOver) return;
        }
    }

    // ===== Post-turn handling =====
    if (gameOver) {
        notifyObservers();
        return;
    }

    bool heavyAutoDrop = p.hasJustDropped(); // check if heavy effect caused drop
    if (heavyAutoDrop) p.clearJustDropped();

    if (droppedByCommand || heavyAutoDrop) {
        switchTurns(); // next player's turn
        notifyObservers();
    }

    notifyObservers();
}

//read from file to get sequence of commands from a file, handle the one by one
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

    notifyObservers();

    string line;
    while (!gameOver && getline(cin, line)) {
        handleCommand(line);
    }

    end();
}

//game over, output scores
void GameEngine::end() {
    cout << "Game over.\n";
    cout << "Final Score P1: " << players[0].getScore() << ", Hi Score: " << players[0].getHiScore() << endl;
    cout << "Final Score P2: " << players[1].getScore() << ", Hi Score: " << players[1].getHiScore() << endl;
}


// takes cmd, expands it via CommandInterpreter, and xecutes each part of the cmd using executeSingleCommand
void GameEngine::handleCommand(const string& cmd) {
    if (gameOver) return;

    string expanded = ci.parse(cmd);
    if (expanded.empty()) return;

    istringstream iss{expanded};
    string token;

    while (iss >> token && !gameOver) {

        //use the commands provided in the file
        if (token == "sequence") {
            string filename;
            if (!(iss >> filename)) break;
            executeSequenceFile(filename);
        }

        //use the sequence of blocks specified in the file
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
