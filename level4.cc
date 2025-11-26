export module Level4;

import Level;
import <string>;

export class Level4 : public Level {
    private:
        int levelNumber;
        int noClearStreak;
        bool randomMode = true;
    public:
        Level4();

        Block* getNextBlock() override;
        bool isHeavy() const override;

        void onBlockPlaced(bool clearedRows) override;

        void readFile(std::string file) override;
        void genBlocksFromFile() override;
        void setRandom(bool enabled) override;
};
