export module SBlock;

import Block;

export class SBlock : public Block {
public:
    SBlock(int level);
    char getVal() const override;
};

