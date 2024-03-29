
#pragma once
#include <Dungeon.hh>
#include <iostream>
#include <vector>

using CoordVector = std::vector<std::pair<int, int>>;

class Builder
{
protected:
    Dungeon* dungeon;
    std::vector<CoordVector> *lineContainer;
    bool** lockedCells;
    std::pair<int, int> currPos;
    
    virtual void buildRoom(
        int y, 
        int x
    );
    
    virtual void initLockedCellsMap(); // done in constructor
    
    virtual std::pair<int, int> setStartPosAtEntry(); 
    
    virtual void buildInitialLine(
        std::pair<int, int> &currPos,
        CoordVector *line
    );
    
    virtual int pickLine();
    
    virtual void addLineToContainer(
        CoordVector* line
    );
    
    virtual void deleteLineFromContainer(
        const int& lineID
    );
    
    virtual std::pair<int, int> pickPos(
        const CoordVector* line
    );
    
    virtual bool isVertical(
        const CoordVector *line
    );
    
    virtual bool isHorizontal(
        const CoordVector *line
    );    

    virtual void scanAndLockAroundPos(
        const std::pair<int, int> &currPos
    );
    
    virtual int randRange(
        const int distance
    );
    
    virtual int getFreeRight(
        const std::pair<int, int> &currPos
    );
    
    virtual int getFreeLeft(
        const std::pair<int, int> &currPos
    );
    
    virtual int getFreeUp(
        const std::pair<int, int> &currPos
    );
    
    virtual int getFreeDown(
        const std::pair<int, int> &currPos
    );
    
    virtual int lookRight(
        const std::pair<int, int> &currPos
    );
    
    virtual bool buildRight(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        const int range
    );
    
    virtual bool moveRight(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    
    virtual int lookLeft(
        const std::pair<int, int> &currPos
    );
    
    virtual bool buildLeft(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        const int range
    );
    
    virtual bool moveLeft(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );

    virtual int lookUp(
        const std::pair<int, int> &currPos
    );
    
    virtual bool buildUp(
        const std::pair< int, int > &currPos, 
        CoordVector* line, 
        const int range
    );
    
    virtual bool moveUp(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    
    virtual int lookDown(
        const std::pair<int, int> &currPos
    );
    
    virtual bool buildDown(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        const int range
    );
    
    virtual bool moveDown(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    
    virtual bool tryExtend(
        std::pair< int, int > &currPos, 
        CoordVector* line, 
        int &lineID
    );
    
    virtual bool extend(
        const std::pair<int, int> &currPos,
        CoordVector *line
    ); 
    
    virtual bool extendLong(
        std::pair< int, int > &currPos,
        CoordVector* line
    );
    
    virtual void removeDepricatedCells(
        CoordVector *line
    );
    
    virtual void removeDuplicatesInLine(
        CoordVector* line, 
        std::pair< int, int > pos
    );
    
    virtual void displayLockedCells(
        const unsigned int H, 
        const unsigned int W
    );
    
    virtual void displayLineContainerContents();
 
public:
    virtual ~Builder() {};
     
    virtual void initNewDungeon(
        int threashold, 
        int heightLimit, 
        int widthLimit 
    ) = 0;
    
    virtual void buildEntrance() = 0;
    
    virtual void generateLayout() = 0;
    
    virtual void placeEnemies() = 0;
    
    virtual void placeTraps() = 0;
    
    virtual void placeLoot() = 0;
    
    virtual Dungeon* returnResult();
};
