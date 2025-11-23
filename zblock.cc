export module ZBlock;

import Block;

export class ZBlock : public Block {
public:
    ZBlock(int level);
    char getVal() const override;
};

