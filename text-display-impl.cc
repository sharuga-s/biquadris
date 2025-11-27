module TextDisplay;

import Grid;
import Block;
import Cell;
import GameEngine;
import Player;

import <iostream>;
import <string>;
import <vector>;

using namespace std;

TextDisplay::TextDisplay() {}

TextDisplay::TextDisplay(GameEngine* g) : game{g} {}

void TextDisplay::notify() {
    if (!game) return;
    
    // Pull data from the Subject (GameEngine)
    const Player& p1 = game->getPlayer1();
    const Player& p2 = game->getPlayer2();

    currBlock1 = p1.getCurrentBlock();
    currBlock2 = p2.getCurrentBlock();
    
    // Cache the data for rendering
    renderBoard(p1.getGrid(), p2.getGrid(), 
                p1.getLevel(), p2.getLevel(),
                p1.getBlind(), p2.getBlind());
    
    renderNext(p1.getNextBlock(), p2.getNextBlock());
    
    renderScores(p1.getScore(), p2.getScore(), 
                 p1.getHiScore(), p2.getHiScore());
    
    // Actually render to cout
    update();
}

void TextDisplay::renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2, bool blind1, bool blind2) {
    grid1 = &g1;
    grid2 = &g2;
    level1 = lvl1;
    level2 = lvl2;
    isBlind1 = blind1;
    isBlind2 = blind2;
}

void TextDisplay::renderNext(Block* next1, Block* next2) {
    nextBlock1 = next1;
    nextBlock2 = next2;
}

void TextDisplay::renderScores(int s1, int s2, int hi1, int hi2) {
    score1 = s1;
    score2 = s2;
    hiScore1 = hi1;
    hiScore2 = hi2;
}

void TextDisplay::update() {
    if (!grid1 || !grid2) return;
    
    const int width = grid1->getCols();   // should be 11
    const int boardPrintWidth = width * 2; // char + space per column
    const string sep = "       ";        // spacing between the two boards

    // Print scores and levels
    cout << "Level:    " << level1;
    cout << "         Level:    " << level2 << endl;
    
    cout << "Score:    " << score1;
    cout << "         Score:    " << score2 << endl;
    
    cout << "Hi Score: " << hiScore1;
    cout << "         Hi Score: " << hiScore2 << endl;

    // Column numbers with spaces between columns
    for (int c = 0; c < width; ++c) {
        cout << (c % 10);
        if (c != width - 1) cout << ' ';
    }
    cout << sep;
    for (int c = 0; c < width; ++c) {
        cout << (c % 10);
        if (c != width - 1) cout << ' ';
    }
    cout << endl;

    // Top border, matching the spaced columns
    for (int c = 0; c < width; ++c) {
        cout << '-';
        if (c != width - 1) cout << ' ';
    }
    cout << sep;
    for (int c = 0; c < width; ++c) {
        cout << '-';
        if (c != width - 1) cout << ' ';
    }
    cout << endl;
    
    // Get the grid cells
    const auto& cells1 = grid1->getCells();
    const auto& cells2 = grid2->getCells();
    
    // Print the boards side by side (rows 3-17, skip top 3 reserve rows)
    for (int r = 3; r < 18; ++r) {
        // Player 1 board
        for (int c = 0; c < width; ++c) {
            char ch;
            if (isBlind1 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                ch = '?';
            } else {
                ch = cells1[r][c].getVal();
            }
            cout << ch;
            if (c != width - 1) cout << ' ';
        }
        
        cout << sep;  // Space between boards
        
        // Player 2 board
        for (int c = 0; c < width; ++c) {
            char ch;
            if (isBlind2 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                ch = '?';
            } else {
                ch = cells2[r][c].getVal();
            }
            cout << ch;
            if (c != width - 1) cout << ' ';
        }
        
        cout << endl;
    }
    
    // Bottom border
    for (int c = 0; c < width; ++c) {
        cout << '-';
        if (c != width - 1) cout << ' ';
    }
    cout << sep;
    for (int c = 0; c < width; ++c) {
        cout << '-';
        if (c != width - 1) cout << ' ';
    }
    cout << endl;
    
    // ===== NEXT BLOCKS =====
    const int previewWidth = 4; // 4 columns for previews (next/current)

    // Align each player's preview under their own "Next:" header
    // P2 "Next:" starts where its preview starts: previewWidth + sep.size()
    int nextHeaderPad = (previewWidth + static_cast<int>(sep.size())) - 5; // 5 = len("Next:")
    if (nextHeaderPad < 1) nextHeaderPad = 1;

    cout << "Next:" << string(nextHeaderPad, ' ') << "Next:" << endl;

    if (nextBlock1 || nextBlock2) {
        int maxRow1 = 0, maxRow2 = 0;

        if (nextBlock1) {
            for (auto [r, c] : nextBlock1->getCells()) {
                if (r > maxRow1) maxRow1 = r;
            }
        }
        if (nextBlock2) {
            for (auto [r, c] : nextBlock2->getCells()) {
                if (r > maxRow2) maxRow2 = r;
            }
        }

        int rowsToPrint = maxRow1;
        if (maxRow2 > rowsToPrint) rowsToPrint = maxRow2;

        for (int r = 0; r <= rowsToPrint; ++r) {
            // P1 next block in a 4-wide area
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (nextBlock1) {
                    for (auto [br, bc] : nextBlock1->getCells()) {
                        if (br == r && bc == c) {
                            ch = nextBlock1->getVal();
                            break;
                        }
                    }
                }
                cout << ch;
            }

            cout << sep;

            // P2 next block
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (nextBlock2) {
                    for (auto [br, bc] : nextBlock2->getCells()) {
                        if (br == r && bc == c) {
                            ch = nextBlock2->getVal();
                            break;
                        }
                    }
                }
                cout << ch;
            }

            cout << endl;
        }
    }

    cout << endl;

    // ===== CURRENT BLOCKS =====
    // Align each player's current block under their own "Current:" header
    // Same idea as for "Next:"
    int currentHeaderPad = (previewWidth + static_cast<int>(sep.size())) - 8; // 8 = len("Current:")
    if (currentHeaderPad < 1) currentHeaderPad = 1;

    cout << "Current:" << string(currentHeaderPad, ' ') << "Current:" << endl;

    if (currBlock1 || currBlock2) {
        int maxRow1 = 0, maxRow2 = 0;

        if (currBlock1) {
            for (auto [r, c] : currBlock1->getCells()) {
                if (r > maxRow1) maxRow1 = r;
            }
        }
        if (currBlock2) {
            for (auto [r, c] : currBlock2->getCells()) {
                if (r > maxRow2) maxRow2 = r;
            }
        }

        int rowsToPrint = maxRow1;
        if (maxRow2 > rowsToPrint) rowsToPrint = maxRow2;

        for (int r = 0; r <= rowsToPrint; ++r) {
            // P1 current block in a 4-wide area
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (currBlock1) {
                    for (auto [br, bc] : currBlock1->getCells()) {
                        if (br == r && bc == c) {
                            ch = currBlock1->getVal();
                            break;
                        }
                    }
                }
                cout << ch;
            }

            cout << sep;

            // P2 current block
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (currBlock2) {
                    for (auto [br, bc] : currBlock2->getCells()) {
                        if (br == r && bc == c) {
                            ch = currBlock2->getVal();
                            break;
                        }
                    }
                }
                cout << ch;
            }

            cout << endl;
        }
    }

    cout << endl;
}
