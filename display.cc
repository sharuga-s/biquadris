export module Display;

import Grid;
import Block;

// Observer interface - displays observe game state
export class Display {
public:
    virtual ~Display() = default;
    
    // Observer pattern: called by Subject when state changes
    virtual void notify() = 0;
};
