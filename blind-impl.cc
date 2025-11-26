module Blind;

void Blind::apply(Player& p, Grid& g) override {
    p.setBlind(true);
    g.applyBlind();   // or mark blind region for display
}