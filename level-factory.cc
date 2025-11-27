export module LevelFactory;

import Level;
import <memory>;
import <string>;

using namespace std;

export class LevelFactory {
public:
    // Create a level based on level number
    static unique_ptr<Level> createLevel(int levelNum, const string& sequenceFile = "");
    
    // Create a level and preserve the sequence file from an existing level
    static unique_ptr<Level> createLevelPreservingSequence(int levelNum, Level* currentLevel);
};
