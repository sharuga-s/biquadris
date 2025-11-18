## Main Classes
1. Player
    ### movements
    - moveBlock(const string &dir)
    - rotateBlock(bool clockwise)
    - dropBlock()
    - holdBlock()
    - genBlock()
    - placeBlock()
    ### attributes
    - score -> linked to observer
    - setLevel()
    - incLevel()
    - decLevel()
    - isHeavy()
    ### board controls
    - grid()
    - reset()
    - newBlock() -> could be handled by the board instead
    - applySpecialEffect(Effect*)
2. Game Engine
    - start()
    - altTurns()
    - getScore()
    - restart()
    - applySpecialAction(Player& source, Player& target, Action a)
3. Grid
    ### check valid & grid controls
    - isValid() -> check all positions of T by calling cells of each of Cell
    - isEnd() 
    - reset()
    ### check rows
    - isRowFull()
    - eraseRow()
    - what()
    ### see whole grid
    - getCells()
    ### track turns
    - getTurn()
    - incTurn()
    - resetTurn()
    ### level features  
    - applyBlind(bool)
    - dropStarBlock()
    - collapseRows()
4. Cell
    - getRow()
    - getCol()
    - setRow()
    - setCol()
    - getVal()
    - setVal()
    - isOccupied()
5. Block
    - left()
    - right()
    - down()
    - place()
    - rotateCW()
    - rotateCCW()
    - getCells()
    - setCells()
    - getVal()
6. Level
    - readFile()
    - getLevel()
    - genBlocksFromFile()
    - getNextBlock()
    - Block* generate()
    - bool isHeavy()
    - void notifyBlockPlaced(bool clearedRows)
7. Command Interpreter
    - parse()
    - readSequenceFile()
    - applyMultiplier()
8. Special Action
    - apply(Player &p)


### Derived Classes
1. Decorator Class for Block
    - iBlock
    - lBlock
    - jBlock
    - oBlock
    - sBlock
    - zBlock
    - tBlock
    - starBlock
2. Decorator Class for Level
    - Level0
    - Level1
    - Level2
    - Level3
    - Level4
3. Decorator Class for Special Action
    - Blind
    - Heavy
    - Force