export module Level3;

import Level;

import <string>;

export class Level3 : public Level {
    private:
        int levelNumber;
        bool randomMode = true;
    public:
        Level3();

        char generateNextBlockType() override;
        bool isHeavy() const override;

        void setRandom(bool enabled) override;
};
