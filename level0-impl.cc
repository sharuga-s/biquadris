module Level0;

import <fstream>;
import <vector>;
import <string>;

using namespace std;

// Level 0: (Make sure you at least get this one working!) Takes its blocks in sequence
// from the files biquadris sequence1.txt (for player 1) and biquadris sequence2.txt (for
// player 2) (samples are provided), or from other files, whose names are supplied on the com-
// mand line. If you get to the end of one of these files, and the game hasn’t ended yet, begin
// reading the file again from the beginning. This level is non-random, and can be used to test
// with a predetermined set of blocks. Make sure that sequence[1-2].txt, and any other
// sequence files you intend to use with your project, are submitted to Marmoset
// along with your code.

Level0::Level0(string file) : sequence{}, index{0}, filename{file}, levelNumber{0} {
    if (!file.empty()) {
        genBlocksFromFile();
    }
}

string Level0::getFileName() const {
    return filename;
}

bool Level0::isHeavy() const {
    return false;
}

void Level0::readFile(string file) {
    filename = file;
    genBlocksFromFile();
}

void Level0::genBlocksFromFile() {
    sequence.clear();
    index = 0;

    ifstream in{filename};
    char ch;

    while (in >> ch) {
        sequence.push_back(ch);
    }
}

void Level0::onBlockPlaced(bool clearedRows) {
    // Level 0 does nothing
}

char Level0::generateNextBlockType() {
    if (sequence.empty()) {
        return '\0'; //means “no block available”
    }

    char type = sequence[index];
    //wrap-around logic --> if we reach the end of the file, go back to the start
    index = (index + 1) % sequence.size();
    return type;
}

void Level0::setRandom(bool enabled) {
}

void Level0::setSequenceFile(const std::string& file) {
    readFile(file);
}

string Level0::getSequenceFile() const {
    return filename;
}

