export module IBlock;

import Block;

export class IBlock : public Block {
    public:
        IBlock(int level);
        char getVal() const override;
};
