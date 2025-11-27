export module Blind;

import Player;
import Grid;
import SpecialAction;

export class Blind : public SpecialAction {
    public:
       void apply(Player& p, Grid& g) override;
}