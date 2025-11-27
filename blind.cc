export module Blind;

import SpecialAction;
import Player;
import Grid;

export class Blind : public SpecialAction {
    public:
       void apply(Player& p, Grid& g) override;
};
