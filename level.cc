export module Level;

import Block;

import <string>;

// note: might need an impl file bc w/o it its bad design...

export class Level {
public:
    virtual ~Level() = default;

    virtual Block* getNextBlock() = 0;

    // depends on Level number — Level 3 & 4 are heavy
    virtual bool isHeavy() const = 0;

    // called after a block is placed (to keep track of how many rows are cleared, if any)
    virtual void onBlockPlaced(bool clearedRows) = 0;

    // load sequence file for Level 0 or Level 1 (if custom)
    virtual void readFile(std::string filename) = 0;

    virtual void genBlocksFromFile() = 0;
};
