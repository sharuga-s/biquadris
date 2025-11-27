module Heavy;

import Player;
import Grid;

void Heavy::apply(Player& p, Grid&) {
    p.incrementHeavyEffects();
}