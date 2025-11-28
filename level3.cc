export module Level3;

import Level;

import <string>;
import <vector>;

using namespace std;

export class Level3 : public Level {
    private:
        int levelNumber;
        bool randomMode = true;
        vector<char> sequence;
        int sequenceIndex = 0;
        string filename;
        
        void genBlocksFromFile();

    public:
        Level3();

        char generateNextBlockType() override;
        bool isHeavy() const override;
        void onBlockPlaced(bool clearedRows) override;
        void setRandom(bool enabled) override;
        void readFile(string file) override;
        void setSequenceFile(const std::string& file) override;
        string getSequenceFile() const override;
        bool shouldDropStar() const override;
};
