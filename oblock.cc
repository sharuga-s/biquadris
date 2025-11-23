export module OBlock;

import Block;

export class OBlock : public Block {
public:
    OBlock(int level);
    char getVal() const override;
};

