module GameEngine;

import Player;
import TextDisplay;
import GraphicDisplay;
import CommandInterpreter;

import <iostream>;
import <fstream>;
import <sstream>;
import <string>;
import <utility>;
import <vector>;

using namespace std;

Player& GameEngine::currentPlayer() {
    return players[currPlayer];
}

Player& GameEngine::otherPlayer() {
    return players[1 - currPlayer];
}

void GameEngine::switchTurns() {
    currPlayer = 1 - currPlayer;
}

// this will need a lot of refactoring once we work on Display...
void GameEngine::updateDisplays() {
    // text
    textDisplay.renderBoard(players[0].getGrid(), players[1].getGrid(), players[0].getLevel(), players[1].getLevel(), players[0].getBlind(), players[1].getBlind());
    textDisplay.renderNext(players[0].getNextBlock(), players[1].getNextBlock());
    textDisplay.renderScores(players[0].getScore(), players[1].getScore(), players[0].getHiScore(), players[1].getHiScore());
    textDisplay.update();

    // graphics
    graphicsDisplay.renderBoard(players[0].getGrid(), players[1].getGrid(), players[0].getLevel(), players[1].getLevel(), players[0].getBlind(), players[1].getBlind());
    graphicsDisplay.renderNext(players[0].getNextBlock(), players[1].getNextBlock());
    graphicsDisplay.renderScores(players[0].getScore(), players[1].getScore(), players[0].getHiScore(), players[1].getHiScore());
    graphicsDisplay.update();
}

void GameEngine::executeSingleCommand(const std::string& cmd) {
    Player& p = currentPlayer();

    // 1. player movement
    if (cmd == "left") {
        p.moveLeft();
    }
    else if (cmd == "right") {
        p.moveRight();
    }
    else if (cmd == "down") {
        p.moveDown();
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
    } else if (cmd == "hold") { // hold
        p.holdBlock();
    } else if (cmd == "levelup") {            // level up
        p.incLevel();
    } else if (cmd == "leveldown") {          // level down
        p.decLevel();
    } else if (cmd == "random") {             // random generation (for levels that support it)
        p.setRandomMode(true);
    } else if (cmd == "norandom") {           // fixed sequence / no random
        p.setRandomMode(false);
    } else if (cmd == "I" || cmd == "J" || cmd == "L" ||
               cmd == "O" || cmd == "S" || cmd == "Z" || cmd == "T") { 
        // SPECIAL ACTION: FORCE NEXT BLOCK on the *other* player
        otherPlayer().forceNextBlock(cmd[0]);
    }

    // igore unknown commands

    updateDisplays();
}

void GameEngine::executeSequenceFile(const std::string& filename) {
    ifstream in{filename};
    if (!in) {
        cerr << "Cannot open sequence file: " << filename << '\n'; // fix ts
        return;
    }

    string line;
    while (getline(in, line) && !gameOver) {
        handleCommand(line);
    }
}

GameEngine::GameEngine(Player p1, Player p2, TextDisplay td, GraphicDisplay gd)
    : currPlayer{0}, players{}, ci{}, textDisplay{move(td)}, graphicsDisplay{move(gd)}, gameOver{false} {
        players.push_back(move(p1));
        players.push_back(move(p2));
}

void GameEngine::start() {
    gameOver = false;
    currPlayer = 0;

    updateDisplays();

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

void GameEngine::handleCommand(const std::string& rawCmd) {
    if (gameOver) return;

    // needs work: once ci implemented, we need to fix ts if needed!
    string expanded = ci.parse(rawCmd);
    if (expanded.empty()) return;

    istringstream iss{expanded};
    string token;

    while (iss >> token && !gameOver) {
        if (token == "sequence") {
            string filename;
            if (!(iss >> filename)) break;
            executeSequenceFile(filename);
        } else {
            executeSingleCommand(token);
        }
    }
}


