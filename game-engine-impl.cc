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


void GameEngine::executeSingleCommand(const string& cmd) {
    Player& p = currentPlayer();
    bool droppedByCommand = false;   // ✅ did THIS command drop a block?

    // 1. player movement / actions
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
        droppedByCommand = true;    // ✅ mark that we dropped

        if (p.getGameOver()) {
            gameOver = true;
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
        for (auto& pl : players) {
            pl.reset();
        }
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

    // If game ended from this command, do NOT switch turns
    if (gameOver) {
        notifyObservers();
        return;
    }

    // ✅ For blocks auto-dropped by Heavy during left/right/down
    bool heavyAutoDrop = p.hasJustDropped();
    if (heavyAutoDrop) {
        p.clearJustDropped();
    }

    // ✅ Exactly ONE turn switch if *either*:
    //   - user typed "drop"
    //   - or Heavy caused an auto-drop inside move
    if (droppedByCommand || heavyAutoDrop) {
        switchTurns();
    }

    if (!gameOver) {
        Player &p = currentPlayer();
        if (p.hasJustDropped()) {
            p.clearJustDropped();
            switchTurns();
        }
    }


    // OBSERVER PATTERN: Notify all observers after state change!
    notifyObservers();
}


// void GameEngine::executeSingleCommand(const string& cmd) {
//     Player& p = currentPlayer();
//     bool droppedByCommand = false;   // ✅ did THIS command drop a block?

//     // 1. player movement / actions
//     if (cmd == "left") {
//         p.moveBlockLeft();
//     }
//     else if (cmd == "right") {
//         p.moveBlockRight();
//     }
//     else if (cmd == "down") {
//         p.moveBlockDown();
//     }
//     else if (cmd == "clockwise") {
//         p.rotateCW();
//     }
//     else if (cmd == "counterclockwise") {
//         p.rotateCCW();
//     }
//     else if (cmd == "drop") {
//         p.dropBlock();
//         droppedByCommand = true;    // ✅ mark that we dropped

//         if (p.getGameOver()) {
//             gameOver = true;
//         }
//     }
//     else if (cmd == "hold") {
//         p.holdBlock();
//     }
//     else if (cmd == "levelup") {
//         p.incLevel();
//     }
//     else if (cmd == "leveldown") {
//         p.decLevel();
//     }
//     else if (cmd == "random") {
//         p.setRandomMode(true);
//     }
//     else if (cmd == "restart") {
//         for (auto& pl : players) {
//             pl.reset();
//         }
//         gameOver = false;
//         currPlayer = 0;
//     }
//     else if (cmd == "blind") {
//         if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
//             Blind effect;
//             otherPlayer().applyEffect(&effect);
//             p.useOneSpecialAction();
//         }
//     }
//     else if (cmd == "heavy") {
//         if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
//             Heavy effect;
//             otherPlayer().applyEffect(&effect);
//             p.useOneSpecialAction();
//         }
//     }
//     else if (cmd == "I" || cmd == "J" || cmd == "L" ||
//              cmd == "O" || cmd == "S" || cmd == "Z" || cmd == "T") {
//         if (p.hasSpecialAction() && p.getNumSpecialActions() > 0) {
//             otherPlayer().forceNextBlock(cmd[0]);
//             p.useOneSpecialAction();
//         }
//     }

//     // If game ended from this command, do NOT switch turns
//     if (gameOver) {
//         notifyObservers();
//         return;
//     }

//     // ✅ For blocks auto-dropped by Heavy during left/right/down
//     bool heavyAutoDrop = p.hasJustDropped();
//     if (heavyAutoDrop) {
//         p.clearJustDropped();
//     }

//     // ✅ Exactly ONE turn switch if *either*:
//     //   - user typed "drop"
//     //   - or Heavy caused an auto-drop inside move
//     if (droppedByCommand || heavyAutoDrop) {
//         switchTurns();
//     }

//     // OBSERVER PATTERN: Notify all observers after state change!
//     notifyObservers();
// }

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

void GameEngine::end() {
    cout << "Game over.\n";
    cout << "Final Score P1: " << players[0].getScore() << '\n';
    cout << "Final Score P2: " << players[1].getScore() << '\n';
}

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
