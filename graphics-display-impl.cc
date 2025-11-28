module GraphicsDisplay;

import GameEngine;
import Player;
import Grid;
import Block;
import xwindow;

import <memory>;
import <vector>;
import <string>;
import <algorithm>;  // for min/max

using namespace std;

// ------------------------
//   Constructor
// ------------------------

GraphicsDisplay::GraphicsDisplay(bool textOnly)
    : game{nullptr}, xw{nullptr}, textOnly{textOnly}
{
    prevBoard1.assign(rows, vector<char>(cols, '\0'));
    prevBoard2.assign(rows, vector<char>(cols, '\0'));

    if (!textOnly) {
        xw = make_unique<Xwindow>(windowWidth, windowHeight);
        initGraphics();
    }
}

GraphicsDisplay::GraphicsDisplay(GameEngine* g, bool textOnly)
    : GraphicsDisplay{textOnly}
{
    game = g;
}

// ------------------------
//   Initialization
// ------------------------

void GraphicsDisplay::initGraphics() {
    if (!xw) return;
    clearBackground();

    xw->drawString(board1Left, 30, "Player 1");
    xw->drawString(board2Left, 30, "Player 2");
}

void GraphicsDisplay::clearBackground() {
    if (!xw) return;
    xw->fillRectangle(0, 0, windowWidth, windowHeight, Xwindow::White);
}

// ------------------------
//   Cell Rendering
// ------------------------

void GraphicsDisplay::drawCell(int boardIndex, int row, int col, char type, bool cellBlind) {
    if (!xw) return;
    if (row < visibleTopRow || row >= rows) return;

    int baseX = (boardIndex == 0 ? board1Left : board2Left);
    int x = baseX + col * cellSize;
    int y = boardTop + (row - visibleTopRow) * cellSize;

    int colour;

    if (cellBlind) {
        colour = Xwindow::Brown;
    } else {
        switch (type) {
            case 'I': colour = Xwindow::Cyan;    break;
            case 'J': colour = Xwindow::Blue;    break;
            case 'L': colour = Xwindow::Orange;  break;
            case 'O': colour = Xwindow::Yellow;  break;
            case 'S': colour = Xwindow::Green;   break;
            case 'Z': colour = Xwindow::Red;     break;
            case 'T': colour = Xwindow::Magenta; break;
            case '*': colour = Xwindow::Brown;   break;
            case ' ': colour = Xwindow::White;   break;
            default:  colour = Xwindow::White;   break;
        }
    }

    // Draw border
    xw->fillRectangle(x, y, cellSize, cellSize, Xwindow::Black);
    // Draw inner cell
    xw->fillRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, colour);
}

// ------------------------
//   Board Rendering
// ------------------------

void GraphicsDisplay::renderBoards(
    const Grid& g1,
    const Grid& g2,
    Block* curr1,
    Block* curr2,
    bool blind1,
    bool blind2
) {
    const auto& c1 = g1.getCells();
    const auto& c2 = g2.getCells();

    // Get current block positions for overlay
    vector<pair<int,int>> currPos1, currPos2;
    if (curr1) currPos1 = curr1->getAbsoluteCells();
    if (curr2) currPos2 = curr2->getAbsoluteCells();

    // OPTIMIZATION: Only redraw cells that changed (dirty rectangle tracking)
    for (int r = visibleTopRow; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {

            bool blindCell1 = blind1 && r >= 3 && r <= 12 && c >= 3 && c <= 9;
            bool blindCell2 = blind2 && r >= 3 && r <= 12 && c >= 3 && c <= 9;

            // Check if current block occupies this position
            char display1 = c1[r][c].getVal();
            char display2 = c2[r][c].getVal();
            
            if (curr1) {
                for (auto [br, bc] : currPos1) {
                    if (br == r && bc == c) {
                        display1 = curr1->getVal();
                        break;
                    }
                }
            }
            
            if (curr2) {
                for (auto [br, bc] : currPos2) {
                    if (br == r && bc == c) {
                        display2 = curr2->getVal();
                        break;
                    }
                }
            }

            char effective1 = blindCell1 ? '?' : display1;
            char effective2 = blindCell2 ? '?' : display2;

            // Only draw if changed
            if (prevBoard1[r][c] != effective1) {
                drawCell(0, r, c, display1, blindCell1);
                prevBoard1[r][c] = effective1;
            }
            if (prevBoard2[r][c] != effective2) {
                drawCell(1, r, c, display2, blindCell2);
                prevBoard2[r][c] = effective2;
            }
        }
    }

    prevBlind1 = blind1;
    prevBlind2 = blind2;
}

// ------------------------
//   Next Block Rendering
// ------------------------

void GraphicsDisplay::renderNext(Block* b1, Block* b2) {
    if (!xw) return;

    // OPTIMIZATION: Only redraw if next blocks changed
    bool block1Changed = (b1 != prevNextBlock1);
    bool block2Changed = (b2 != prevNextBlock2);

    if (!block1Changed && !block2Changed) {
        return;  // Nothing changed, skip redraw
    }

    int nextY = boardTop + visibleRows * cellSize + 30;
    int areaHeight = cellSize * 4 + 30;

    // Lambda to draw next block preview
    auto drawNext = [&](Block* blk, int boardIndex, bool changed) {
        if (!changed) return;  // Skip if this specific block didn't change

        int baseX = (boardIndex == 0 ? board1Left : board2Left);
        
        // Clear only this board's next block area
        xw->fillRectangle(baseX, nextY - 20, cols * cellSize, areaHeight, Xwindow::White);
        
        // Draw "Next:" label
        xw->drawString(baseX, nextY, "Next:");

        if (!blk) return;

        const auto& cells = blk->getCells();
        if (cells.empty()) return;

        int minR = 999, minC = 999;

        // Find bounding box
        for (auto [r, c] : cells) {
            minR = min(minR, r);
            minC = min(minC, c);
        }

        int originX = baseX + cellSize;
        int originY = nextY + 10;

        char val = blk->getVal();

        int colour = Xwindow::White;
        switch (val) {
            case 'I': colour = Xwindow::Cyan;    break;
            case 'J': colour = Xwindow::Blue;    break;
            case 'L': colour = Xwindow::Orange;  break;
            case 'O': colour = Xwindow::Yellow;  break;
            case 'S': colour = Xwindow::Green;   break;
            case 'Z': colour = Xwindow::Red;     break;
            case 'T': colour = Xwindow::Magenta; break;
            case '*': colour = Xwindow::Brown;   break;
        }

        // Draw block cells
        for (auto [r, c] : cells) {
            int rr = r - minR;
            int cc = c - minC;

            int x = originX + cc * cellSize;
            int y = originY + rr * cellSize;

            xw->fillRectangle(x, y, cellSize, cellSize, Xwindow::Black);
            xw->fillRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, colour);
        }
    };

    // Only redraw changed blocks
    drawNext(b1, 0, block1Changed);
    drawNext(b2, 1, block2Changed);

    // Update cache
    prevNextBlock1 = b1;
    prevNextBlock2 = b2;
}

// ------------------------
//   Held Block Rendering
// ------------------------

void GraphicsDisplay::renderHeld(Block* h1, Block* h2) {
    if (!xw) return;

    // OPTIMIZATION: Only redraw if held blocks changed
    bool block1Changed = (h1 != prevHeldBlock1);
    bool block2Changed = (h2 != prevHeldBlock2);

    if (!block1Changed && !block2Changed) {
        return;  // Nothing changed, skip redraw
    }

    // Position held block display below next block
    int nextY = boardTop + visibleRows * cellSize + 30;
    int nextAreaHeight = cellSize * 4 + 30;
    int heldY = nextY + nextAreaHeight + 10;  // Below next block
    int areaHeight = cellSize * 4 + 30;

    // Lambda to draw held block preview
    auto drawHeld = [&](Block* blk, int boardIndex, bool changed) {
        if (!changed) return;  // Skip if this specific block didn't change

        int baseX = (boardIndex == 0 ? board1Left : board2Left);
        
        // Clear only this board's held block area
        xw->fillRectangle(baseX, heldY - 20, cols * cellSize, areaHeight, Xwindow::White);
        
        // Draw "Hold:" label
        xw->drawString(baseX, heldY, "Hold:");

        if (!blk) return;  // No held block

        const auto& cells = blk->getCells();
        if (cells.empty()) return;

        int minR = 999, minC = 999;

        // Find bounding box
        for (auto [r, c] : cells) {
            minR = min(minR, r);
            minC = min(minC, c);
        }

        int originX = baseX + cellSize;
        int originY = heldY + 10;

        char val = blk->getVal();

        int colour = Xwindow::White;
        switch (val) {
            case 'I': colour = Xwindow::Cyan;    break;
            case 'J': colour = Xwindow::Blue;    break;
            case 'L': colour = Xwindow::Orange;  break;
            case 'O': colour = Xwindow::Yellow;  break;
            case 'S': colour = Xwindow::Green;   break;
            case 'Z': colour = Xwindow::Red;     break;
            case 'T': colour = Xwindow::Magenta; break;
            case '*': colour = Xwindow::Brown;   break;
        }

        // Draw block cells
        for (auto [r, c] : cells) {
            int rr = r - minR;
            int cc = c - minC;

            int x = originX + cc * cellSize;
            int y = originY + rr * cellSize;

            xw->fillRectangle(x, y, cellSize, cellSize, Xwindow::Black);
            xw->fillRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, colour);
        }
    };

    // Only redraw changed blocks
    drawHeld(h1, 0, block1Changed);
    drawHeld(h2, 1, block2Changed);

    // Update cache
    prevHeldBlock1 = h1;
    prevHeldBlock2 = h2;
}

// ------------------------
//   Score Rendering
// ------------------------

void GraphicsDisplay::renderScores(
    int s1, int s2,
    int hi1, int hi2,
    int lvl1, int lvl2
) {
    if (!xw) return;

    // OPTIMIZATION: Skip if nothing changed
    if (s1 == prevScore1 && s2 == prevScore2 &&
        hi1 == prevHi1 && hi2 == prevHi2 &&
        lvl1 == prevLevel1 && lvl2 == prevLevel2) return;

    int scoreBand = boardTop - 20;
    xw->fillRectangle(0, 0, windowWidth, scoreBand, Xwindow::White);

    string p1a = "Player 1";
    string p1b = "Level: " + to_string(lvl1) +
                 "  Score: " + to_string(s1) +
                 "  Hi: " + to_string(hi1);

    string p2a = "Player 2";
    string p2b = "Level: " + to_string(lvl2) +
                 "  Score: " + to_string(s2) +
                 "  Hi: " + to_string(hi2);

    xw->drawString(board1Left, 25, p1a);
    xw->drawString(board1Left, 45, p1b);

    xw->drawString(board2Left, 25, p2a);
    xw->drawString(board2Left, 45, p2b);

    prevScore1 = s1;
    prevScore2 = s2;
    prevHi1 = hi1;
    prevHi2 = hi2;
    prevLevel1 = lvl1;
    prevLevel2 = lvl2;
}

// ------------------------
//   Observer Notify
// ------------------------

void GraphicsDisplay::notify() {
    if (textOnly || !xw || !game) return;

    const Player &p1 = game->getPlayer1();
    const Player &p2 = game->getPlayer2();

    renderBoards(p1.getGrid(), p2.getGrid(), 
                 p1.getCurrentBlock(), p2.getCurrentBlock(),
                 p1.getBlind(), p2.getBlind());
    renderNext(p1.getNextBlock(), p2.getNextBlock());
    renderHeld(p1.getHeldBlock(), p2.getHeldBlock());
    renderScores(
        p1.getScore(), p2.getScore(),
        p1.getHiScore(), p2.getHiScore(),
        p1.getLevel(), p2.getLevel()
    );
}
