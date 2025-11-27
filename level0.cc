export module Level0;

import Level;
import <vector>;
import <string>;

using namespace std;

export class Level0 : public Level {
private:
    vector<char> sequence;
    int index = 0;           // where we're at in sequence
    string filename;
    int levelNumber;

public:
    explicit Level0(string file);

    char generateNextBlockType() override;
    bool isHeavy() const override;
    void onBlockPlaced(bool clearedRows) override;

    void readFile(string file) override;
    void genBlocksFromFile() override;
    void setRandom(bool enabled) override;
    string getFileName() const override;
};
