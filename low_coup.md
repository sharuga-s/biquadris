# **1. GameEngine**

### **Responsibilities**

* `void start()`
* `void restart()`
* `void altTurns()`
* `void handleCommand(std::string cmd)`
* `void updateDisplays()`
* `int getHighScore() const`

### **Relationships**

* has → `Player` p1, p2
* has → `CommandInterpreter`
* calls → `Display::update()`
* **does not interact directly with Grid or Level**

---

# **2. Player**

### **Movement**

* `void moveBlockLeft()`
* `void moveBlockRight()`
* `void moveBlockDown()`
* `void rotateBlockCW()`
* `void rotateBlockCCW()`
* `void dropBlock()`
* `void holdBlock()`

### **Block Management**

* `Block* getCurrentBlock()`
* `Block* getNextBlock()`
* `Block* getHeldBlock()`
* `void generateNewBlock()`   // uses Level::getNextBlock()
* `void placeCurrentBlock()`

### **Scoring + Leveling**

* `int getScore() const`
* `void addScore(int delta)`
* `void setLevel(int lv)`
* `void incLevel()`
* `void decLevel()`
* `void applyEffect(SpecialAction* effect)`

### **Board Controls**

* `Grid& getGrid()`
* `void reset()`

### **State Flags**

* `bool isBlind() const`
* `bool isHeavy() const`

### **Relationships**

* owns → `Grid`
* owns → `Block` (current, next, held)
* has → `Level` strategy
* receives → `SpecialAction`
* **sole mediator to Grid**

---

# **3. Grid**

### **Validation + Core**

* `bool isValid(Block* b) const`
* `bool isGameOver() const`
* `void reset()`
* `const std::vector<std::vector<Cell>>& getCells() const`

### **Row Logic**

* `bool isRowFull(int row) const`
* `void eraseRow(int row)`
* `void collapseRows()`

### **Special Mechanics**

* `void applyBlind(bool on)`
* `void placeBlock(Block* b)`   // block placement
* `void clearFullRows(int& numCleared)`

### **Turn Tracking**

* `int getTurn() const`
* `void incTurn()`
* `void resetTurn()`

### **Relationships**

* contains → `Cell`
* subject-of → `Display` (observer)
* **does not talk to Level or GameEngine**

---

# **4. Cell**

* `int getRow() const`
* `int getCol() const`
* `void setRow(int r)`
* `void setCol(int c)`
* `char getVal() const`
* `void setVal(char v)`
* `bool isOccupied() const`

---

# **5. Block (abstract)**

### **Movement**

* `void moveLeft()`
* `void moveRight()`
* `void moveDown()`
* `void place(Grid&)`

### **Rotation**

* `void rotateCW()`
* `void rotateCCW()`

### **Cell Access**

* `const std::vector<Cell>& getCells() const`
* `void setCells(std::vector<Cell> cells)`
* `char getVal() const`

### **State**

* `bool isHeavy() const`
* `void setHeavy(bool)`
* `int getGenerationLevel() const`

### **Relationships**

* composed-of → Cell[]
* subclassed by → IBlock, JBlock, ..., StarBlock

---

# **6. Level (interface)**

* `Block* getNextBlock()`
* `bool isHeavy() const`
* `void onBlockPlaced(bool clearedRows)`
* `void readFile(std::string filename)`
* `void genBlocksFromFile()`

### **Relationships**

* implemented by → Level0–Level4
* used by → Player
* **never manipulates Grid directly**

---

# **7. CommandInterpreter**

* `std::string parse(const std::string& cmd)`
* `std::string expandShortcuts(const std::string& cmd)`
* `std::string applyMultiplier(const std::string& cmd)`
* `void executeSequenceFile(const std::string& filename)`
* `std::string resolveAbbreviations(const std::string& cmd)`

### **Relationships**

* used only by → GameEngine
* knows nothing about Grid/Player

---

# **8. SpecialAction (abstract)**

* `virtual void apply(Player& p) = 0`

### **Subclasses**

#### **Blind**

* `void apply(Player& p)` — sets blind mode until next drop

#### **Heavy**

* `void apply(Player& p)` — increases heavy flag

#### **Force**

* `void apply(Player& p)` — forces replacement block

---

# **9. Display (interface)**

* `void renderBoard(const Grid&)`
* `void renderNext(const Block&)`
* `void renderScore(int)`
* `void update()`

### **Subclasses**

* `TextDisplay`
* `GraphicsDisplay`

### **Relationships**

* observes → Grid
* updated by → GameEngine
