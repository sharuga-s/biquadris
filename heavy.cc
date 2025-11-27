export module Heavy;

import Player;
import Grid;

export class Heavy : public SpecialAction {
public:
    void apply(Player& p, Grid&) override;
};
