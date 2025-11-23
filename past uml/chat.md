# **Main Classes**

## **1. GameEngine**

### responsibilities

* start()
* restart()
* altTurns()
* applySpecialAction()
* handleCommand()
* getHighScore()
* updateDisplays()

### relationships

* has two Players
* has CommandInterpreter
* has Display (Text + Graphics)
* mediates between Player, Grid, Level

---

## **2. Player**

### movement

* moveBlock()
* rotateBlock()
* dropBlock()
* holdBlock()

### block management

* genBlock()
* placeBlock()
* getCurrentBlock()
* getNextBlock()
* getHeldBlock()

### scoring + leveling

* score
* getScore()
* setLevel()
* incLevel()
* decLevel()
* applyEffect(SpecialAction*)

### board controls

* grid()
* reset()

### state

* isBlind()
* isHeavy()

### relationships

* owns a Grid
* owns currentBlock, nextBlock, heldBlock
* has a Level strategy
* receives SpecialAction effects

---

## **3. Grid**

### validation + core

* isValid(Block*)
* isEnd()
* reset()
* getCells()

### row logic

* isRowFull()
* eraseRow()
* collapseRows()

### special mechanics

* applyBlind(bool)
* dropStarBlock()      // Level 4 mechanic

### turn tracking

* getTurn()
* incTurn()
* resetTurn()

### relationships

* contains Cells
* observed by Displays

---

## **4. Cell**

* getRow()
* getCol()
* setRow()
* setCol()
* getVal()
* setVal()
* isOccupied()

---

## **5. Block (abstract)**

### movement

* left()
* right()
* down()
* place()

### rotation

* rotateCW()
* rotateCCW()

### cell access

* getCells()
* setCells()
* getVal()

### state

* heavy flag
* type char (I,J,L,O,S,Z,T,*)
* generationLevel

### relationships

* composed of Cells
* subclassed into concrete block types

---

## **6. Level (interface)**

* getNextBlock()
* isHeavy()
* onBlockPlaced(bool clearedRows)
* readFile()
* genBlocksFromFile()

### relationships

* implemented by Level0–Level4
* used by Player to generate new Blocks
* Level4 triggers star blocks via Grid

---

# ## **Derived Block Classes (Concrete Tetrominoes)**

* IBlock
* JBlock
* LBlock
* OBlock
* SBlock
* ZBlock
* TBlock
* StarBlock (1×1 block)

Each overrides:

* getCells() initial shape
* type char

*(If you plan a decorator pattern instead, rename “Decorator Block” → “Block Decorator,” but the tetra shapes still appear as subclasses.)*

---

# ## **Derived Level Classes**

* Level0

  * sequential file looping

* Level1

  * skewed probabilities (S,Z heavier prob)

* Level2

  * uniform random generation

* Level3

  * heavy blocks always
  * automatic down movement after any move

* Level4

  * Level3 behaviour plus
  * star block after 5 blocks without clearing
  * dropStarBlock() triggered via Grid

*(If using decorators: wrap Level0–4 in a Decorator chain.)*

---

# ## **Additional Required System Classes**

---

## **7. CommandInterpreter**

### responsibilities

* parse(string command)
* applyMultiplier()
* executeSequenceFile()
* expandShortcuts()
* resolveAbbreviations()

### notes

* keeps GameEngine clean
* supports future “rename” or “macro” features

---

## **8. SpecialAction (abstract)**

### purpose

* models **blind**, **heavy**, **force**
* allows clean, polymorphic application of effects

### method

* apply(Player&)

---

## **9. SpecialAction Subclasses**

### **Blind**

* apply(): player enters blind mode until next drop

### **Heavy**

* apply(): increments heavy counter

### **Force**

* apply(): replaces player’s current block

---

## **10. Display (interface)**

### responsibilities

* renderBoard(Grid)
* renderNext(Block)
* renderScore(int)
* update()

### relationships

* observed by Grid and GameEngine

---

## **11. TextDisplay : Display**

## **12. GraphicsDisplay : Display**

* uses X11
* colour-coded blocks
