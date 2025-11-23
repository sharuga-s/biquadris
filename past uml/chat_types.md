# ## **1. GameEngine**

### **public methods**

* `void start()`
* `void restart()`
* `void altTurns()`
* `void applySpecialAction(SpecialAction *action, Player &target)`
* `void handleCommand(const std::string &cmd)`
* `int getHighScore() const`
* `void updateDisplays()`

### **private fields**

* `Player players[2]`
* `int currentTurn`
* `int highScore`
* `CommandInterpreter *cmdInterp`
* `Display *textDisp`
* `Display *graphicsDisp`

---

# ## **2. Player**

### **public methods**

* `void moveBlock(const std::string &direction)`

* `void rotateBlock(bool clockwise)`

* `void dropBlock()`

* `void holdBlock()`

* `Block* genBlock()`

* `void placeBlock()`

* `Block* getCurrentBlock() const`

* `Block* getNextBlock() const`

* `Block* getHeldBlock() const`

* `int getScore() const`

* `void setLevel(int lvl)`

* `void incLevel()`

* `void decLevel()`

* `Grid& grid()`

* `void reset()`

* `void applyEffect(SpecialAction *effect)`

* `bool isBlind() const`

* `bool isHeavy() const`

### **private fields**

* `Grid board`

* `Block *currentBlock`

* `Block *nextBlock`

* `Block *heldBlock`

* `Level *levelStrategy`

* `int score`

* `int level`

* `bool blindActive`

* `int heavyCount`        // stacked heavy effects

* `std::vector<SpecialAction*> activeEffects`

---

# ## **3. Grid**

### **public methods**

* `bool isValid(const Block *b) const`

* `bool isEnd() const`

* `void reset()`

* `std::vector<std::vector<Cell>>& getCells()`

* `bool isRowFull(int row) const`

* `void eraseRow(int row)`

* `void collapseRows()`

* `void applyBlind(bool active)`

* `void dropStarBlock()`

* `int getTurn() const`

* `void incTurn()`

* `void resetTurn()`

### **private fields**

* `std::vector<std::vector<Cell>> cells`
* `bool blindMode`
* `int turnCounter`

---

# ## **4. Cell**

### **public methods**

* `int getRow() const`

* `int getCol() const`

* `void setRow(int r)`

* `void setCol(int c)`

* `char getVal() const`

* `void setVal(char v)`

* `bool isOccupied() const`

### **private fields**

* `int row`
* `int col`
* `char value`     // '.' empty, or block type

---

# ## **5. Block (abstract)**

### **public methods**

* `void left()`

* `void right()`

* `void down()`

* `void place(Grid &g)`

* `void rotateCW()`

* `void rotateCCW()`

* `std::vector<Cell> getCells() const`

* `void setCells(const std::vector<Cell> &c)`

* `char getVal() const`

* `bool isHeavy() const`

* `int getGenerationLevel() const`

### **protected fields**

* `std::vector<Cell> cells`
* `char type`
* `bool heavy`
* `int generationLevel`

---

# ## **6. Level (interface)**

### **public methods**

* `virtual Block* getNextBlock() = 0`

* `virtual bool isHeavy() const = 0`

* `virtual void onBlockPlaced(bool clearedRows) = 0`

* `virtual int getLevel() const = 0`

* `virtual void readFile(const std::string &filename)`

* `virtual void genBlocksFromFile()`

### **protected fields**

* none (interface)

---

# ## **7. Derived Block Classes**

### **IBlock, JBlock, LBlock, OBlock, SBlock, ZBlock, TBlock, StarBlock**

* override initial `cells`
* set correct `type`
* StarBlock: 1×1 shape

---

# ## **8. Derived Level Classes**

### **Level0**

* file-based deterministic generation
* loops file

### **Level1**

* weighted probabilities (S/Z more common)

### **Level2**

* uniform random

### **Level3**

* heavy blocks
* automatic down movement after every move

### **Level4**

* Level3 behaviour + star block mechanic (track 5 placed)

### **fields for Level0–4**

* `int levelNumber`
* `std::vector<char> sequence`
* `int index`
* `int blocksSinceClear`   // Level4 logic

---

# ## **9. CommandInterpreter**

### **public methods**

* `void parse(const std::string &cmd)`
* `std::string readSequenceFile(const std::string &filename)`
* `std::pair<int,std::string> applyMultiplier(const std::string &cmd)`
* `std::string resolveAbbreviation(const std::string &cmd)`

### **private fields**

* `std::map<std::string,std::string> commandMap`
* `std::vector<std::string> sequenceBuffer`
* `bool readingSequence`

---

# ## **10. SpecialAction (abstract)**

### **public methods**

* `virtual void apply(Player &target) = 0`

### **subclasses**

#### **Blind**

* apply(): activates blind mode

#### **Heavy**

* apply(): increments heavy counter

#### **Force**

* apply(): replace target player’s current block

### **private fields**

* For Force:

  * `char forcedType`

---

# ## **11. Display (interface)**

### **public methods**

* `virtual void renderBoard(const Grid&) = 0`
* `virtual void renderNext(const Block*) = 0`
* `virtual void renderScore(int current, int high) = 0`
* `virtual void update() = 0`

---

# ## **12. TextDisplay : Display**

### **private fields**

* `std::vector<std::string> buffer`

---

# ## **13. GraphicsDisplay : Display**

### **private fields**

* `X11 window handle`
* any drawing buffers needed

