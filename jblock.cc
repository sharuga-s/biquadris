export module JBlock;

import Block;

export class JBlock : public Block {
public:
    JBlock(int level);
    char getVal() const override;
};
