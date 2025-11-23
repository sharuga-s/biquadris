export module LBlock;

import Block;

export class LBlock : public Block {
public:
    LBlock(int level);
    char getVal() const override;
};
