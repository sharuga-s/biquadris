export module Level0;

import Level;
import Block;
import <vector>;
import <string>;

using namespace std;

export class Level0 : public Level {
    private:
        vector<char> sequence; //stores block types from the sequence file --> ** we use char instead of Block **
        int index = 0; //where we're at in sequence
        string filename;
        int levelNumber = 0;

    public:
        Level0(std::string file);

        Block* getNextBlock() override;
        bool isHeavy() const override;
        void onBlockPlaced(bool clearedRows) override;

        void readFile(string file) override;
        void genBlocksFromFile() override;
        void setRandom(bool enabled) override;
};
