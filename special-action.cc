export module SpecialAction;

import Player;
import Grid;

/*
relationship:
- HAS-A [aggregation] Grid
*/

export class SpecialAction {
    public:
        virtual ~SpecialAction() = default;
        virtual void apply(Player& p, Grid& g) = 0;
};