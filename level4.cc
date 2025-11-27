export module Level4;

import Level;

import <string>;
import <vector>;

using namespace std;

export class Level4 : public Level {
    private:
        int levelNumber;
        int noClearStreak;
        bool randomMode = true;
        vector<char> sequence;
        int sequenceIndex = 0;
        string filename;

        void genBlocksFromFile();

    public:
        Level4();

        char generateNextBlockType() override;
        bool isHeavy() const override;
        void onBlockPlaced(bool clearedRows) override;
        void setRandom(bool enabled) override;
        void readFile(string file) override;
        void setSequenceFile(const string& file) override;
        string getSequenceFile() const override;
};
