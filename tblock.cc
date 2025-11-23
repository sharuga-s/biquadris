export module TBlock;

import Block;

export class TBlock : public Block {
public:
    TBlock(int level);
    char getVal() const override;
};
