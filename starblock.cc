export module StarBlock;

import Block;

export class StarBlock : public Block {
public:
    StarBlock(int level);
    char getVal() const override;
};
