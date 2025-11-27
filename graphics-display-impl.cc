module GraphicsDisplay;

import Display;
import Grid;
import Block;
import Cell;
import GameEngine;
import Player;
import xwindow;

import <memory>;
import <string>;

using namespace std;

GraphicsDisplay::GraphicsDisplay(bool textOnly) 
    : textOnly{textOnly} {
    if (!textOnly) {
        xw = make_unique<Xwindow>(800, 600);
    }
}

GraphicsDisplay::GraphicsDisplay(GameEngine* g, bool textOnly) 
    : game{g}, textOnly{textOnly} {
    if (!textOnly) {
        xw = make_unique<Xwindow>(800, 600);
    }
}

void GraphicsDisplay::notify() {
    if (!game) return;
    
    // Pull data from the Subject (GameEngine)
    const Player& p1 = game->getPlayer1();
    const Player& p2 = game->getPlayer2();
    
    // Cache the data for rendering
    renderBoard(p1.getGrid(), p2.getGrid(), 
                p1.getLevel(), p2.getLevel(),
                p1.getBlind(), p2.getBlind());
    
    renderNext(p1.getNextBlock(), p2.getNextBlock());
    
    renderScores(p1.getScore(), p2.getScore(), 
                 p1.getHiScore(), p2.getHiScore());
    
    // Actually render to X11 window
    update();
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

int GraphicsDisplay::getColorForType(char type) {
    switch (type) {
        case 'I': return Xwindow::Cyan;
        case 'J': return Xwindow::Blue;
        case 'L': return Xwindow::Orange;
        case 'O': return Xwindow::Yellow;
        case 'S': return Xwindow::Green;
        case 'Z': return Xwindow::Red;
        case 'T': return Xwindow::Magenta;
        case '*': return Xwindow::Brown;
        case '?': return Xwindow::Black;  // For blind effect
        default: return Xwindow::White;
    }
}

void GraphicsDisplay::drawCell(int x, int y, char type) {
    if (!xw || textOnly) return;
    
    int color = getColorForType(type);
    xw->fillRectangle(x, y, cellSize, cellSize, color);
}

void GraphicsDisplay::drawBlock(Block* block, int offsetX, int offsetY) {
    if (!block || !xw || textOnly) return;
    
    const auto& cells = block->getCells();
    char type = block->getVal();
    
    for (const auto& [r, c] : cells) {
        int x = offsetX + c * cellSize;
        int y = offsetY + r * cellSize;
        drawCell(x, y, type);
    }
}

void GraphicsDisplay::update() {
    if (!xw || textOnly) return;
    if (!grid1 || !grid2) return;
    
    // Clear the window with white background
    xw->fillRectangle(0, 0, 800, 600, Xwindow::White);
    
    // Starting positions for the two boards
    int board1X = 50;
    int board2X = 450;
    int boardY = 150;
    
    // Draw scores and levels at the top
    xw->drawString(board1X, 30, "Player 1");
    xw->drawString(board1X, 50, "Level: " + to_string(level1));
    xw->drawString(board1X, 70, "Score: " + to_string(score1));
    xw->drawString(board1X, 90, "Hi: " + to_string(hiScore1));
    
    xw->drawString(board2X, 30, "Player 2");
    xw->drawString(board2X, 50, "Level: " + to_string(level2));
    xw->drawString(board2X, 70, "Score: " + to_string(score2));
    xw->drawString(board2X, 90, "Hi: " + to_string(hiScore2));
    
    // Draw Player 1's board
    const auto& cells1 = grid1->getCells();
    for (int r = 3; r < 18; ++r) {  // Skip top 3 reserve rows
        for (int c = 0; c < 11; ++c) {
            int x = board1X + c * cellSize;
            int y = boardY + (r - 3) * cellSize;
            
            char cellType;
            if (isBlind1 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                cellType = '?';  // Blind effect
            } else {
                cellType = cells1[r][c].getVal();
            }
            
            if (cellType != ' ') {
                drawCell(x, y, cellType);
            }
        }
    }
    
    // Draw Player 2's board
    const auto& cells2 = grid2->getCells();
    for (int r = 3; r < 18; ++r) {
        for (int c = 0; c < 11; ++c) {
            int x = board2X + c * cellSize;
            int y = boardY + (r - 3) * cellSize;
            
            char cellType;
            if (isBlind2 && r >= 3 && r <= 12 && c >= 3 && c <= 9) {
                cellType = '?';
            } else {
                cellType = cells2[r][c].getVal();
            }
            
            if (cellType != ' ') {
                drawCell(x, y, cellType);
            }
        }
    }
    
    // Draw "Next" blocks
    int nextY = boardY + 15 * cellSize + 30;
    xw->drawString(board1X, nextY, "Next:");
    xw->drawString(board2X, nextY, "Next:");
    
    if (nextBlock1) {
        drawBlock(nextBlock1, board1X, nextY + 10);
    }
    
    if (nextBlock2) {
        drawBlock(nextBlock2, board2X, nextY + 10);
    }
}
