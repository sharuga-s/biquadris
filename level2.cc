export module Level2;

import Level;
import <string>;

using namespace std;

export class Level2 : public Level {
    private:
        int levelNumber;
    
    public:
        Level2();

        char generateNextBlockType() override;
        bool isHeavy() const override;

        void setRandom(bool enabled) override;
};
