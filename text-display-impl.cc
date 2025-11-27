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
    const Player& p1 = game->currentPlayer();
    const Player& p2 = game->otherPlayer();
    
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
    
    // Print scores and levels
    cout << "Level:    " << level1;
    cout << "         Level:    " << level2 << endl;
    
    cout << "Score:    " << score1;
    cout << "         Score:    " << score2 << endl;
    
    cout << "Hi Score: " << hiScore1;
    cout << "         Hi Score: " << hiScore2 << endl;
    
    cout << "-----------       -----------" << endl;
    
    // Get the grid cells
    const auto& cells1 = grid1->getCells();
    const auto& cells2 = grid2->getCells();
    
    // Print the boards side by side (rows 3-17, skip top 3 reserve rows)
    for (int r = 3; r < 18; ++r) {
        // Player 1 board
        for (int c = 0; c < 11; ++c) {
            if (isBlind1 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                cout << '?';
            } else {
                cout << cells1[r][c].getVal();
            }
        }
        
        cout << "       ";  // Space between boards
        
        // Player 2 board
        for (int c = 0; c < 11; ++c) {
            if (isBlind2 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                cout << '?';
            } else {
                cout << cells2[r][c].getVal();
            }
        }
        
        cout << endl;
    }
    
    cout << "-----------       -----------" << endl;
    
    // Print "Next:" blocks
    cout << "Next:             Next:" << endl;
    
    // Render next blocks (simplified - just show the type)
    if (nextBlock1) {
        // Get the cells of the next block
        const auto& block1Cells = nextBlock1->getCells();
        
        // Find dimensions
        int maxRow = 0, maxCol = 0;
        for (const auto& [r, c] : block1Cells) {
            if (r > maxRow) maxRow = r;
            if (c > maxCol) maxCol = c;
        }
        
        // Print block
        for (int r = 0; r <= maxRow; ++r) {
            for (int c = 0; c <= maxCol; ++c) {
                bool found = false;
                for (const auto& [br, bc] : block1Cells) {
                    if (br == r && bc == c) {
                        cout << nextBlock1->getVal();
                        found = true;
                        break;
                    }
                }
                if (!found) cout << ' ';
            }
            
            // Space and player 2's next block
            if (nextBlock2) {
                cout << "       ";  // Space between
                
                const auto& block2Cells = nextBlock2->getCells();
                int maxRow2 = 0, maxCol2 = 0;
                for (const auto& [r2, c2] : block2Cells) {
                    if (r2 > maxRow2) maxRow2 = r2;
                    if (c2 > maxCol2) maxCol2 = c2;
                }
                
                if (r <= maxRow2) {
                    for (int c = 0; c <= maxCol2; ++c) {
                        bool found = false;
                        for (const auto& [br, bc] : block2Cells) {
                            if (br == r && bc == c) {
                                cout << nextBlock2->getVal();
                                found = true;
                                break;
                            }
                        }
                        if (!found) cout << ' ';
                    }
                }
            }
            
            cout << endl;
        }
    }
    
    cout << endl;
}