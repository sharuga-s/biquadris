module GraphicsDisplay;

import GameEngine;
import Player;
import Grid;
import Block;
import xwindow;

import <memory>;
import <vector>;
import <string>;
import <algorithm>;  

using namespace std;

// constants to avoid magic numbers

// the gui Y positions
const int PLAYER_LABEL_Y = 30;
const int SCORE_TITLE_Y = 25;
const int SCORE_STATS_Y = 45;

// spacing/layout for gui
const int NEXT_AREA_OFFSET_Y = 30; // gap before "Next:"
const int NEXT_CLEAR_PADDING = 20; // clear area padding
const int GAP_AND_OFFSET = 10; // spacing between next + held

// blind area boundaries 
const int BLIND_ROW_START = 3;
const int BLIND_ROW_END = 12;
const int BLIND_COL_START = 3;
const int BLIND_COL_END = 9;

// cleaner rendering
const int MAX_BOUND_INIT = 999;  
const int PREVIEW_SIZE = 4;

// ---------------------------------


// ctor
GraphicsDisplay::GraphicsDisplay(bool textOnly) : game{nullptr}, xw{nullptr}, textOnly{textOnly}
{
    prevBoard1.assign(rows, vector<char>(cols, '\0'));
    prevBoard2.assign(rows, vector<char>(cols, '\0'));

    if (!textOnly) {
        xw = make_unique<Xwindow>(windowWidth, windowHeight);
        initGraphics();
    }
}

GraphicsDisplay::GraphicsDisplay(GameEngine* g, bool textOnly) : GraphicsDisplay{textOnly}
{
    game = g;
}

// purpose: initializing graphics
void GraphicsDisplay::initGraphics() {
    if (!xw) return;
    clearBackground();

    xw->drawString(board1Left, PLAYER_LABEL_Y, "Player 1");
    xw->drawString(board2Left, PLAYER_LABEL_Y, "Player 2");
}

void GraphicsDisplay::clearBackground() {
    if (!xw) return;
    xw->fillRectangle(0, 0, windowWidth, windowHeight, Xwindow::White);
}

// purpose: render cells with correct colours
void GraphicsDisplay::drawCell(int boardIndex, int row, int col, char type, bool cellBlind) {
    if (!xw) return;
    if (row < visibleTopRow || row >= rows) return;

    int baseX = (boardIndex == 0 ? board1Left : board2Left);
    int x = baseX + col * cellSize;
    int y = boardTop + (row - visibleTopRow) * cellSize;

    int colour = cellBlind ? Xwindow::Black : getColorForBlockType(type);

    // draw border
    xw->fillRectangle(x, y, cellSize, cellSize, Xwindow::Black);
    // draw inner cell
    xw->fillRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, colour);
}

// purpose: get color for block type
int GraphicsDisplay::getColorForBlockType(char type) {
    switch (type) {
        case 'I': return Xwindow::Cyan;
        case 'J': return Xwindow::Blue;
        case 'L': return Xwindow::Orange;
        case 'O': return Xwindow::Yellow;
        case 'S': return Xwindow::Green;
        case 'Z': return Xwindow::Red;
        case 'T': return Xwindow::Magenta;
        case '*': return Xwindow::Brown;
        default: return Xwindow::White;
    }
}

// purpose: render boards for players
void GraphicsDisplay::renderBoards(const Grid& g1, const Grid& g2, Block* curr1, Block* curr2, bool blind1, bool blind2) {
    const auto& c1 = g1.getCells();
    const auto& c2 = g2.getCells();

    // get curr block positions for overlay
    vector<pair<int,int>> currPos1, currPos2;
    if (curr1) currPos1 = curr1->getAbsoluteCells();
    if (curr2) currPos2 = curr2->getAbsoluteCells();

    // OPTIMIZATION: only redraw cells that changed bc too slow before
    for (int r = visibleTopRow; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {

            bool blindCell1 = blind1 && r >= BLIND_ROW_START && r <= BLIND_ROW_END && c >= BLIND_COL_START && c <= BLIND_COL_END;
            bool blindCell2 = blind2 && r >= BLIND_ROW_START && r <= BLIND_ROW_END && c >= BLIND_COL_START && c <= BLIND_COL_END;

            // check if curr block occupies this position
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

            // only draw if changed
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

// purpose: helper to draw a single next block preview
void GraphicsDisplay::drawNextBlockPreview(Block* blk, int boardIndex, int nextY) {
    if (!blk) return;

    const auto& cells = blk->getCells();
    if (cells.empty()) return;

    int minR = MAX_BOUND_INIT, minC = MAX_BOUND_INIT;

    // find bounding box
    for (auto [r, c] : cells) {
        minR = min(minR, r);
        minC = min(minC, c);
    }

    int baseX = (boardIndex == 0 ? board1Left : board2Left);
    int originX = baseX + cellSize;
    int originY = nextY + GAP_AND_OFFSET;

    char val = blk->getVal();
    int colour = getColorForBlockType(val);

    // Draw block cells
    for (auto [r, c] : cells) {
        int rr = r - minR;
        int cc = c - minC;

        int x = originX + cc * cellSize;
        int y = originY + rr * cellSize;

        xw->fillRectangle(x, y, cellSize, cellSize, Xwindow::Black);
        xw->fillRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, colour);
    }
}

// purpose: render next block for player
void GraphicsDisplay::renderNext(Block* b1, Block* b2) {
    if (!xw) return;

    // OPTIMIZATION: only redraw if next blocks changed
    bool block1Changed = (b1 != prevNextBlock1);
    bool block2Changed = (b2 != prevNextBlock2);

    if (!block1Changed && !block2Changed) {
        return;  // nothing changed => skip redraw
    }

    int nextY = boardTop + visibleRows * cellSize + PLAYER_LABEL_Y;
    int areaHeight = cellSize * PREVIEW_SIZE + PLAYER_LABEL_Y;

    // draw Player 1's next block if changed
    if (block1Changed) {
        int baseX = board1Left;
        
        // clear this board's next block area
        xw->fillRectangle(baseX, nextY - NEXT_CLEAR_PADDING, cols * cellSize, areaHeight, Xwindow::White);
        
        // draw "Next:" label
        xw->drawString(baseX, nextY, "Next:");

        // draw the block preview
        drawNextBlockPreview(b1, 0, nextY);
    }

    // draw Player 2's next block if changed
    if (block2Changed) {
        int baseX = board2Left;
        
        // clear this board's next block area
        xw->fillRectangle(baseX, nextY - NEXT_CLEAR_PADDING, cols * cellSize, areaHeight, Xwindow::White);
        
        // draw "Next:" label
        xw->drawString(baseX, nextY, "Next:");

        // draw the block preview
        drawNextBlockPreview(b2, 1, nextY);
    }

    // update cache
    prevNextBlock1 = b1;
    prevNextBlock2 = b2;
}

// purpose: helper to draw a single held block preview
void GraphicsDisplay::drawHeldBlockPreview(Block* blk, int boardIndex, int heldY) {
    if (!blk) return;

    const auto& cells = blk->getCells();
    if (cells.empty()) return;

    int minR = MAX_BOUND_INIT, minC = MAX_BOUND_INIT;

    // find bounding box
    for (auto [r, c] : cells) {
        minR = min(minR, r);
        minC = min(minC, c);
    }

    int baseX = (boardIndex == 0 ? board1Left : board2Left);
    int originX = baseX + cellSize;
    int originY = heldY + GAP_AND_OFFSET;

    char val = blk->getVal();
    int colour = getColorForBlockType(val);

    // fraw block cells
    for (auto [r, c] : cells) {
        int rr = r - minR;
        int cc = c - minC;

        int x = originX + cc * cellSize;
        int y = originY + rr * cellSize;

        xw->fillRectangle(x, y, cellSize, cellSize, Xwindow::Black);
        xw->fillRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, colour);
    }
}

// purpose: render held block for player
void GraphicsDisplay::renderHeld(Block* h1, Block* h2) {
    if (!xw) return;

    // OPTIMIZATION: only redraw if held blocks changed
    bool block1Changed = (h1 != prevHeldBlock1);
    bool block2Changed = (h2 != prevHeldBlock2);

    if (!block1Changed && !block2Changed) {
        return;  // nothing changed, skip redraw
    }

    // position held block display below next block
    int nextY = boardTop + visibleRows * cellSize + PLAYER_LABEL_Y;
    int nextAreaHeight = cellSize * PREVIEW_SIZE + PLAYER_LABEL_Y;
    int heldY = nextY + nextAreaHeight + GAP_AND_OFFSET;
    int areaHeight = cellSize * PREVIEW_SIZE + PLAYER_LABEL_Y;

    // draw Player 1's held block if changed
    if (block1Changed) {
        int baseX = board1Left;
        
        // clear this board's held block area
        xw->fillRectangle(baseX, heldY - NEXT_CLEAR_PADDING, cols * cellSize, areaHeight, Xwindow::White);
        
        // draw "Hold:" label
        xw->drawString(baseX, heldY, "Hold:");

        // draw the block preview (if there is one)
        drawHeldBlockPreview(h1, 0, heldY);
    }

    // draw Player 2's held block if changed
    if (block2Changed) {
        int baseX = board2Left;
        
        // clear this board's held block area
        xw->fillRectangle(baseX, heldY - NEXT_CLEAR_PADDING, cols * cellSize, areaHeight, Xwindow::White);
        
        // draw "Hold:" label
        xw->drawString(baseX, heldY, "Hold:");

        // draw the block preview (if there is one)
        drawHeldBlockPreview(h2, 1, heldY);
    }

    // update cache
    prevHeldBlock1 = h1;
    prevHeldBlock2 = h2;
}

// purpose: render scorse
void GraphicsDisplay::renderScores(
    int s1, int s2,
    int hi1, int hi2,
    int lvl1, int lvl2
) {
    if (!xw) return;

    // OPTIMIZATION: skip if nothing changed
    if (s1 == prevScore1 && s2 == prevScore2 &&
        hi1 == prevHi1 && hi2 == prevHi2 &&
        lvl1 == prevLevel1 && lvl2 == prevLevel2) return;

    int scoreBand = boardTop - NEXT_CLEAR_PADDING;
    xw->fillRectangle(0, 0, windowWidth, scoreBand, Xwindow::White);

    string p1a = "Player 1";
    string p1b = "Level: " + to_string(lvl1) + "  Score: " + to_string(s1) + "  Hi: " + to_string(hi1);

    string p2a = "Player 2";
    string p2b = "Level: " + to_string(lvl2) + "  Score: " + to_string(s2) + "  Hi: " + to_string(hi2);

    xw->drawString(board1Left, SCORE_TITLE_Y, p1a);
    xw->drawString(board1Left, SCORE_STATS_Y, p1b);

    xw->drawString(board2Left, SCORE_TITLE_Y, p2a);
    xw->drawString(board2Left, SCORE_STATS_Y, p2b);

    prevScore1 = s1;
    prevScore2 = s2;
    prevHi1 = hi1;
    prevHi2 = hi2;
    prevLevel1 = lvl1;
    prevLevel2 = lvl2;
}

// purpose: obs patt notify method
void GraphicsDisplay::notify() {
    if (textOnly || !xw || !game) return;

    const Player &p1 = game->getPlayer1();
    const Player &p2 = game->getPlayer2();

    renderBoards(p1.getGrid(), p2.getGrid(), p1.getCurrentBlock(), p2.getCurrentBlock(), p1.getBlind(), p2.getBlind());
    renderNext(p1.getNextBlock(), p2.getNextBlock());
    renderHeld(p1.getHeldBlock(), p2.getHeldBlock());
    renderScores( p1.getScore(), p2.getScore(), p1.getHiScore(), p2.getHiScore(), p1.getLevel(), p2.getLevel()
    );
}

void GraphicsDisplay::setGame(GameEngine* g) {
    game = g;
}
