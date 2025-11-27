module GraphicsDisplay;

import Grid;
import Block;
import Cell;
import xwindow;

import <iostream>;
import <string>;
import <memory>;

using namespace std;

GraphicsDisplay::GraphicsDisplay(bool textOnlyMode) : textOnly{textOnlyMode} {
    if (!textOnly) {
        // Create window: 2 boards (11 cells wide each) + spacing + next block area
        // Width: (11 + 5 + 11 + 5) cells * 20 pixels = 640 pixels
        // Height: 15 rows * 20 + 150 for header/footer = 450 pixels
        int width = 32 * cellSize;   // 640 pixels
        int height = 15 * cellSize + 150;  // 450 pixels
        
        xw = make_unique<Xwindow>(width, height);
    }
}

int GraphicsDisplay::getColorForType(char type) {
    // Map block types to Xwindow colors
    switch (type) {
        case 'I': return Xwindow::Cyan;
        case 'J': return Xwindow::Blue;
        case 'L': return Xwindow::Orange;
        case 'O': return Xwindow::Yellow;
        case 'S': return Xwindow::Green;
        case 'Z': return Xwindow::Red;
        case 'T': return Xwindow::Magenta;
        case '*': return Xwindow::Brown;
        case ' ': return Xwindow::White;
        case '?': return Xwindow::Black;  // For blind effect
        default: return Xwindow::White;
    }
}

void GraphicsDisplay::drawCell(int x, int y, char type) {
    if (!xw) return;
    
    int color = getColorForType(type);
    
    // Fill the cell with color
    xw->fillRectangle(x, y, cellSize, cellSize, color);
    
    // Draw border (black outline)
    // We can't draw just an outline with fillRectangle, so we'll skip the border
    // or draw it as thin rectangles if needed
}

void GraphicsDisplay::drawBlock(Block* block, int offsetX, int offsetY) {
    if (!block || !xw) return;
    
    const auto& cells = block->getCells();
    char type = block->getVal();
    
    for (const auto& [r, c] : cells) {
        int x = offsetX + c * cellSize;
        int y = offsetY + r * cellSize;
        drawCell(x, y, type);
    }
}

void GraphicsDisplay::renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2, bool blind1, bool blind2) {
    grid1 = &g1;
    grid2 = &g2;
    level1 = lvl1;
    level2 = lvl2;
    isBlind1 = blind1;
    isBlind2 = blind2;
}

void GraphicsDisplay::renderNext(Block* next1, Block* next2) {
    nextBlock1 = next1;
    nextBlock2 = next2;
}

void GraphicsDisplay::renderScores(int s1, int s2, int hi1, int hi2) {
    score1 = s1;
    score2 = s2;
    hiScore1 = hi1;
    hiScore2 = hi2;
}

void GraphicsDisplay::update() {
    if (textOnly || !xw) return;
    if (!grid1 || !grid2) return;
    
    // Clear window (draw white background)
    xw->fillRectangle(0, 0, 32 * cellSize, 15 * cellSize + 150, Xwindow::White);
    
    int headerHeight = 80;  // Space for scores at top
    
    // Draw scores and levels at top
    xw->drawString(10, 20, "Player 1");
    xw->drawString(10, 40, "Level: " + to_string(level1));
    xw->drawString(10, 60, "Score: " + to_string(score1));
    
    xw->drawString(350, 20, "Player 2");
    xw->drawString(350, 40, "Level: " + to_string(level2));
    xw->drawString(350, 60, "Score: " + to_string(score2));
    
    // Get the grid cells
    const auto& cells1 = grid1->getCells();
    const auto& cells2 = grid2->getCells();
    
    // Draw Player 1 board (rows 3-17, columns 0-10)
    int board1X = 10;
    int boardY = headerHeight;
    
    for (int r = 3; r < 18; ++r) {
        for (int c = 0; c < 11; ++c) {
            int x = board1X + c * cellSize;
            int y = boardY + (r - 3) * cellSize;
            if (isBlind1 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                drawCell(x, y, '?');  // Draw question mark
            } else {
                drawCell(x, y, cells1[r][c].getVal());
            }

        }
    }
    
    // Draw Player 2 board
    int board2X = board1X + 11 * cellSize + 40;
    
    for (int r = 3; r < 18; ++r) {
        for (int c = 0; c < 11; ++c) {
            int x = board2X + c * cellSize;
            int y = boardY + (r - 3) * cellSize;
            if (isBlind2 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                drawCell(x, y, '?');
            } else {
                drawCell(x, y, cells2[r][c].getVal());
            }

        }
    }
    
    // Draw "Next" blocks below the boards
    int nextY = boardY + 15 * cellSize + 20;
    
    xw->drawString(board1X, nextY, "Next:");
    if (nextBlock1) {
        drawBlock(nextBlock1, board1X, nextY + 10);
    }
    
    xw->drawString(board2X, nextY, "Next:");
    if (nextBlock2) {
        drawBlock(nextBlock2, board2X, nextY + 10);
    }
}