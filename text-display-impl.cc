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

string colour(char ch) {
    switch (ch) {
        case 'I': return "\033[36mI\033[0m"; // cyan
        case 'J': return "\033[34mJ\033[0m"; // blue
        case 'L': return "\033[33mL\033[0m"; // yellow
        case 'O': return "\033[97mO\033[0m"; // white
        case 'S': return "\033[32mS\033[0m"; // green
        case 'Z': return "\033[31mZ\033[0m"; // red
        case 'T': return "\033[35mT\033[0m"; // magenta
        case '*': return "\033[38;5;94m*\033[0m"; // brown/orange-brown - couldnt find ansi brown sadge
        case '?': return "\033[90m?\033[0m"; // grey for blind
        default:  return string(1, ch);
    }
}


TextDisplay::TextDisplay() {}

TextDisplay::TextDisplay(GameEngine* g) : game{g} {}

void TextDisplay::notify() {
    if (!game) return;
    
    // pull data from the Subject (GameEngine)
    const Player& p1 = game->getPlayer1();
    const Player& p2 = game->getPlayer2();

    currBlock1 = p1.getCurrentBlock();
    currBlock2 = p2.getCurrentBlock();
    
    // cache the data for rendering
    renderBoard(p1.getGrid(), p2.getGrid(), p1.getLevel(), p2.getLevel(), p1.getBlind(), p2.getBlind());
    
    renderNext(p1.getNextBlock(), p2.getNextBlock());
    renderHeld(p1.getHeldBlock(), p2.getHeldBlock());
    renderScores(p1.getScore(), p2.getScore(), p1.getHiScore(), p2.getHiScore());
    
    // actually render to cout
    update();
}

void TextDisplay::setGame(GameEngine* g) {
    game = g;
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

void TextDisplay::renderHeld(Block* h1, Block* h2) {
    heldBlock1 = h1;
    heldBlock2 = h2;
}

void TextDisplay::renderScores(int s1, int s2, int hi1, int hi2) {
    score1 = s1;
    score2 = s2;
    hiScore1 = hi1;
    hiScore2 = hi2;
}

void TextDisplay::update() {
    if (!grid1 || !grid2) return;
    
    const int width = grid1->getCols(); // should be 11
    const string sep = "       ";  // spacing between the two boards
    const int previewWidth = 21; // 4 columns for previews (next/current)

    // print scores and levels
    cout << "Level:    " << level1;
    cout << "                 Level:    " << level2 << endl;
    
    cout << "Score:    " << score1;
    cout << "                 Score:    " << score2 << endl;
    
    cout << "Hi Score: " << hiScore1;
    cout << "                 Hi Score: " << hiScore2 << endl;

    // column numbers with spaces between columns
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

    // top border, matching the spaced columns
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
    
    // get the grid cells
    const auto& cells1 = grid1->getCells();
    const auto& cells2 = grid2->getCells();
    
    // get current block positions for overlay
    vector<pair<int,int>> currBlockPos1, currBlockPos2;

    if (currBlock1) {
        currBlockPos1 = currBlock1->getAbsoluteCells();
    }
    if (currBlock2) {
        currBlockPos2 = currBlock2->getAbsoluteCells();
    }
    
    // print the boards side by side (rows 0-17 + reserve rows at top)
    for (int r = 0; r < 18; ++r) {
        if (r == 3) {
            // just visual indicator of resevre vs play area
            for (int c = 0; c < width; ++c) {
                cout << '=';
                if (c != width - 1) cout << ' ';
            }
            cout << sep;
            for (int c = 0; c < width; ++c) {
                cout << '=';
                if (c != width - 1) cout << ' ';
            }
            cout << endl;
        }
        
        // player 1 board
        for (int c = 0; c < width; ++c) {
            char ch;
            
            // check if current block occupies this position
            bool isCurrBlock = false;
            if (currBlock1) {
                for (auto [br, bc] : currBlockPos1) {
                    if (br == r && bc == c) {
                        ch = currBlock1->getVal();
                        isCurrBlock = true;
                        break;
                    }
                }
            }
            
            // if not current block, show grid cell (with blind effect if applicable)
            if (!isCurrBlock) {
                if (isBlind1 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                    ch = '?';
                } else {
                    ch = cells1[r][c].getVal();
                }
            }
            
            cout << colour(ch);
            if (c != width - 1) cout << ' ';
        }
        
        cout << sep;  // space between boards
        
        // player 2 board
        for (int c = 0; c < width; ++c) {
            char ch;
            
            // check if current block occupies this position
            bool isCurrBlock = false;
            if (currBlock2) {
                for (auto [br, bc] : currBlockPos2) {
                    if (br == r && bc == c) {
                        ch = currBlock2->getVal();
                        isCurrBlock = true;
                        break;
                    }
                }
            }
            
            // if not current block, show grid cell (with blind effect if applicable)
            if (!isCurrBlock) {
                if (isBlind2 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                    ch = '?';
                } else {
                    ch = cells2[r][c].getVal();
                }
            }
            
            cout << colour(ch);
            if (c != width - 1) cout << ' ';
        }
        
        cout << endl;
    }
    
    // bottom border
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
    
    // next blocks
    int nextHeaderPad = (previewWidth + static_cast<int>(sep.size())) - 5; 
    if (nextHeaderPad < 1) nextHeaderPad = 1;

    cout << "Next:" << string(nextHeaderPad, ' ') << "Next:" << endl;

    if (nextBlock1 || nextBlock2) {
        // find bounding box for each block
        int minRow1 = 999, maxRow1 = -1, minCol1 = 999, maxCol1 = -1;
        int minRow2 = 999, maxRow2 = -1, minCol2 = 999, maxCol2 = -1;

        if (nextBlock1) {
            for (auto [r, c] : nextBlock1->getCells()) {
                if (r < minRow1) minRow1 = r;
                if (r > maxRow1) maxRow1 = r;
                if (c < minCol1) minCol1 = c;
                if (c > maxCol1) maxCol1 = c;
            }
        }
        if (nextBlock2) {
            for (auto [r, c] : nextBlock2->getCells()) {
                if (r < minRow2) minRow2 = r;
                if (r > maxRow2) maxRow2 = r;
                if (c < minCol2) minCol2 = c;
                if (c > maxCol2) maxCol2 = c;
            }
        }

        // 'normalize' to start from (0,0)
        int height1 = (maxRow1 >= 0) ? (maxRow1 - minRow1 + 1) : 0;
        int height2 = (maxRow2 >= 0) ? (maxRow2 - minRow2 + 1) : 0;
        int rowsToPrint = (height1 > height2) ? height1 : height2;

        for (int r = 0; r < rowsToPrint; ++r) {
            // P1 next block in a 4-wide area
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (nextBlock1) {
                    for (auto [br, bc] : nextBlock1->getCells()) {
                        if ((br - minRow1) == r && (bc - minCol1) == c) {
                            ch = nextBlock1->getVal();
                            break;
                        }
                    }
                }
                cout << colour(ch);
            }

            cout << sep;

            // P2 next block
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (nextBlock2) {
                    for (auto [br, bc] : nextBlock2->getCells()) {
                        if ((br - minRow2) == r && (bc - minCol2) == c) {
                            ch = nextBlock2->getVal();
                            break;
                        }
                    }
                }
                cout << colour(ch);
            }

            cout << endl;
        }
    }

    cout << endl;

    // held blocks
    cout << "Held:" << string(nextHeaderPad, ' ') << "Held:" << endl;

    if (heldBlock1 || heldBlock2) {
        // find bounding boxes
        int minRowH1 = 999, maxRowH1 = -1, minColH1 = 999, maxColH1 = -1;
        int minRowH2 = 999, maxRowH2 = -1, minColH2 = 999, maxColH2 = -1;

        if (heldBlock1) {
            for (auto [r, c] : heldBlock1->getCells()) {
                if (r < minRowH1) minRowH1 = r;
                if (r > maxRowH1) maxRowH1 = r;
                if (c < minColH1) minColH1 = c;
                if (c > maxColH1) maxColH1 = c;
            }
        }
        if (heldBlock2) {
            for (auto [r, c] : heldBlock2->getCells()) {
                if (r < minRowH2) minRowH2 = r;
                if (r > maxRowH2) maxRowH2 = r;
                if (c < minColH2) minColH2 = c;
                if (c > maxColH2) maxColH2 = c;
            }
        }

        int heightH1 = (maxRowH1 >= 0) ? (maxRowH1 - minRowH1 + 1) : 0;
        int heightH2 = (maxRowH2 >= 0) ? (maxRowH2 - minRowH2 + 1) : 0;
        int rowsHeld = (heightH1 > heightH2) ? heightH1 : heightH2;

        // print held blocks
        for (int r = 0; r < rowsHeld; ++r) {
            // P1
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (heldBlock1) {
                    for (auto [br, bc] : heldBlock1->getCells()) {
                        if ((br - minRowH1) == r && (bc - minColH1) == c) {
                            ch = heldBlock1->getVal();
                            break;
                        }
                    }
                }
                cout << colour(ch);
            }

            cout << sep;

            // P2
            for (int c = 0; c < previewWidth; ++c) {
                char ch = ' ';
                if (heldBlock2) {
                    for (auto [br, bc] : heldBlock2->getCells()) {
                        if ((br - minRowH2) == r && (bc - minColH2) == c) {
                            ch = heldBlock2->getVal();
                            break;
                        }
                    }
                }
                cout << colour(ch);
            }

            cout << endl;
        }
    }

    cout << endl;
}
