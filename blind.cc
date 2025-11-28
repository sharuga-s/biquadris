export module Blind;

import Player;
import Grid;

export class Blind : public SpecialAction {
    public:
       void apply(Player& p, Grid& g) override; // overrides the method from the SpecialAction class
};
