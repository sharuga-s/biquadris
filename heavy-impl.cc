module Heavy;

import Player;
import Grid;

using namespace std;

void Heavy::apply(Player& p, Grid&) {
     p.setHeavyEffect(true);
}
