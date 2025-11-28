export module Display;

import Grid;
import Block;

// observer interface - displays observe game state
export class Display {
public:
    virtual ~Display() = default;
    
    // observer pattern: called by Subject when state changes
    virtual void notify() = 0;
};
