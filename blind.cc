export module Blind;

export class Blind : public SpecialAction {
    public:
       void apply(Player& p, Grid& g) override;
}