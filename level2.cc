export module Level2;

import Level;
import <string>;

using namespace std;

export class Level2 : public Level {
    private:
        int levelNumber;
    
    public:
        Level2();

        Block* getNextBlock() override;
        bool isHeavy() const override;

        void onBlockPlaced(bool clearedRows) override;

        void readFile(string file) override;
        void genBlocksFromFile() override;
};
