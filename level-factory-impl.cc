module LevelFactory;

import Level0;
import Level1;
import Level2;
import Level3;
import Level4;

import <memory>;
import <string>;

using namespace std;

// purpose: instantiates the correct Level subclass based on the int level number
unique_ptr<Level> LevelFactory::createLevel(int levelNum, const string& sequenceFile) {
    // restrict level to valid range
    if (levelNum < 0) levelNum = 0;
    if (levelNum > 4) levelNum = 4;
    
    switch (levelNum) {
        case 0: {
            // use default sequence file unless caller provided one
            string file = sequenceFile.empty() ? "sequence1.txt" : sequenceFile;
            return make_unique<Level0>(file);
        }
        
        case 1:
            return make_unique<Level1>();
        
        case 2:
            return make_unique<Level2>();
        
        case 3: {
            // heavy behaviour + random distribution
            auto lvl = make_unique<Level3>();
            if (!sequenceFile.empty()) {
                lvl->setSequenceFile(sequenceFile);
            }
            return lvl;
        }
        
        case 4: {
            auto lvl = make_unique<Level4>();
            if (!sequenceFile.empty()) {
                lvl->setSequenceFile(sequenceFile);
            }
            return lvl;
        }
        
        // just in case but should never even reach dis case
        default:
            return make_unique<Level0>("sequence1.txt");
    }
}

// purpose: NEW level object but preserves the sequence file from an existing level
//  why: when user changes level mid-game but still wanst same script
unique_ptr<Level> LevelFactory::createLevelPreservingSequence(int levelNum, Level* currentLevel) {
    string sequenceFile = "";
    
    if (currentLevel) {
        sequenceFile = currentLevel->getSequenceFile();
    }
    
    return createLevel(levelNum, sequenceFile);
}
