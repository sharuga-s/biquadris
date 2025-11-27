module Blind;

import Player;
import Grid;

void Blind::apply(Player& p, Grid& g) {
    p.setBlind(true);
}