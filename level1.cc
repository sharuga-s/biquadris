export module Level1;

import Level;
import <string>;
using namespace std;

export class Level1 : public Level {
    private:
        int levelNumber;

    public:
        Level1();

        char generateNextBlockType() override;
        bool isHeavy() const override;

        void setRandom(bool enabled) override;
};
