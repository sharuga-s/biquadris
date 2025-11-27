module GraphicsDisplay;

import Grid;
import Block;
import Cell;

import <iostream>;
import <string>;

using namespace std;

// For now, this is a stub implementation that does nothing
// You'll need to add X11 headers and linking later

GraphicsDisplay::GraphicsDisplay(bool textOnly) {
    if (textOnly) {
        initialized = false;
        return;
    }
    
    // TODO: Initialize X11 window
    // This requires X11 headers which may not compile without proper setup
    initialized = false;
}

GraphicsDisplay::~GraphicsDisplay() {
    // TODO: Clean up X11 resources
}

void GraphicsDisplay::initX11() {
    // TODO: Create X11 window
    // Example code (requires X11 headers):
    /*
    display = XOpenDisplay(nullptr);
    if (!display) {
        cerr << "Cannot open X11 display" << endl;
        return;
    }
    
    int screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                  0, 0, 800, 600, 1,
                                  BlackPixel(display, screen),
                                  WhitePixel(display, screen));
    
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);
    
    gc = XCreateGC(display, window, 0, nullptr);
    initialized = true;
    */
}

int GraphicsDisplay::getColorForType(char type) {
    // Map block types to colors
    // These would be actual X11 color values
    switch (type) {
        case 'I': return 0; // Cyan
        case 'J': return 1; // Blue
        case 'L': return 2; // Orange
        case 'O': return 3; // Yellow
        case 'S': return 4; // Green
        case 'Z': return 5; // Red
        case 'T': return 6; // Purple
        case '*': return 7; // Brown
        default: return 8;  // White/empty
    }
}

void GraphicsDisplay::drawCell(int x, int y, char type) {
    // TODO: Draw a cell at pixel position (x, y) with color for type
    // Example X11 code:
    /*
    XSetForeground(display, gc, getColorForType(type));
    XFillRectangle(display, window, gc, x, y, cellSize, cellSize);
    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));
    XDrawRectangle(display, window, gc, x, y, cellSize, cellSize);
    */
}

void GraphicsDisplay::renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2) {
    grid1 = &g1;
    grid2 = &g2;
    level1 = lvl1;
    level2 = lvl2;
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
    if (!initialized) return;
    if (!grid1 || !grid2) return;
    
    // TODO: Clear window and redraw everything
    // Example structure:
    /*
    XClearWindow(display, window);
    
    // Draw player 1 board
    const auto& cells1 = grid1->getCells();
    for (int r = 3; r < 18; ++r) {
        for (int c = 0; c < 11; ++c) {
            int x = c * cellSize;
            int y = (r - 3) * cellSize + 50;  // Offset for score display
            drawCell(x, y, cells1[r][c].getVal());
        }
    }
    
    // Draw player 2 board (offset to the right)
    const auto& cells2 = grid2->getCells();
    for (int r = 3; r < 18; ++r) {
        for (int c = 0; c < 11; ++c) {
            int x = (c + 15) * cellSize;  // Offset by 15 cells
            int y = (r - 3) * cellSize + 50;
            drawCell(x, y, cells2[r][c].getVal());
        }
    }
    
    // Draw scores (would need XDrawString)
    // Draw next blocks
    
    XFlush(display);
    */
}
