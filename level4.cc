export module Level4;

import Level;
import <string>;

export class Level4 : public Level {
    private:
        int levelNumber;
        int noClearStreak;
        bool randomMode = true;
        vector<char> sequence;
        int sequenceIndex = 0;
        string filename;
    public:
        Level4();

        char generateNextBlockType() override;
        bool isHeavy() const override;

        void onBlockPlaced(bool clearedRows);

        void setRandom(bool enabled) override;
};
