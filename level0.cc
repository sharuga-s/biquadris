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

    void genBlocksFromFile();
public:
    explicit Level0(string file);

    char generateNextBlockType() override;
    bool isHeavy() const override;

    void readFile(string file) override;
    void setRandom(bool enabled) override;
    void onBlockPlaced(bool clearedRows) override;
    string getFileName() const;

    void setSequenceFile(const std::string& file) override;
    string getSequenceFile() const override;
    virtual bool shouldDropStar() const override;
};
