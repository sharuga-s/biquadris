export module LevelFactory;

import Level;
import <memory>;
import <string>;

using namespace std;

export class LevelFactory {
    public:
        // create a level based on level number
        static unique_ptr<Level> createLevel(int levelNum, const string& sequenceFile = "");
        
        // create a level and keep the sequence file from an existing level
        static unique_ptr<Level> createLevelPreservingSequence(int levelNum, Level* currentLevel);
};
