// level0-impl.cc
module Level0;

import Level0;
import Block;

import IBlock;
import JBlock;
import LBlock;
import OBlock;
import SBlock;
import ZBlock;
import TBlock;

import <fstream>;
import <vector>;w
import <string>;

using namespace std;

Level0::Level0(string file)
    : sequence{}, index{0}, filename{file}, levelNumber{0} 
{
    if (!file.empty()) {
        genBlocksFromFile();
    }
}

bool Level0::isHeavy() const {
    return false;
}

void Level0::onBlockPlaced(bool clearedRows) {
    // Level 0 does nothing
}

void Level0::readFile(string file) {
    filename = file;
    genBlocksFromFile();
}

void Level0::genBlocksFromFile() {
    sequence.clear();
    index = 0;

    ifstream in(filename);
    char ch;

    while (in >> ch) {
        sequence.push_back(ch);
    }
}

Block* Level0::getNextBlock() {
    if (sequence.empty()) {
        return nullptr;
    }

    char type = sequence[index];
    index = (index + 1) % sequence.size();

    if (type == 'I') {
        return new IBlock(levelNumber);
    } 
    else if (type == 'J') {
        return new JBlock(levelNumber);
    } 
    else if (type == 'L') {
        return new LBlock(levelNumber);
    } 
    else if (type == 'O') {
        return new OBlock(levelNumber);
    } 
    else if (type == 'S') {
        return new SBlock(levelNumber);
    } 
    else if (type == 'Z') {
        return new ZBlock(levelNumber);
    } 
    else if (type == 'T') {
        return new TBlock(levelNumber);
    } 
    else {
        return nullptr;
    }
}
