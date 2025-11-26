export module Level3;

import Level;
import <string>;

export class Level3 : public Level {
    private:
        int levelNumber;
        bool randomMode = true;
    public:
        Level3();

        Block* getNextBlock() override;
        bool isHeavy() const override;

        void onBlockPlaced(bool clearedRows) override;

        void readFile(std::string file) override;
        void genBlocksFromFile() override;
        void setRandom(bool enabled) override;
};
