module Level0;

import <fstream>;
import <vector>;
import <string>;

using namespace std;

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

bool Level0::shouldDropStar() const {
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

