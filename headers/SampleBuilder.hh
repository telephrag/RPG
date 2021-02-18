
#pragma once
#include <iostream>
#include <Builder.hh>
#include <unordered_map>

using CoordVector = std::vector<std::pair<int, int>>;

class SampleBuilder: public Builder
{   
private:
    Dungeon* dungeon;
    std::vector<CoordVector> *lineContainer;
    bool** lockedCells;
    std::pair<int, int> currPos;
    
    void buildRoom(
        int y, 
        int x
    );
    
    void initLockedCellsMap();
    
    std::pair<int, int> setStartPosAtEntry();
    
    void buildInitialLine(
        std::pair<int, int> &currPos,
        CoordVector *line
    );
    
    std::pair<int, int> pickPos(
        const CoordVector* line
    );
    
    bool tryExtend(
        std::pair< int, int > &currPos, 
        CoordVector* line, 
        int &lineID
    );
    
    bool extendLong(
        std::pair< int, int > &currPos,
        CoordVector* line
    );
        
    
    int randRange(
        const int distance
    );
    
    int getFreeRight(
        const std::pair<int, int> &currPos
    );
    
    int getFreeLeft(
        const std::pair<int, int> &currPos
    );
    
    int getFreeUp(
        const std::pair<int, int> &currPos
    );
    
    int getFreeDown(
        const std::pair<int, int> &currPos
    );
    
    int lookRight(
        const std::pair<int, int> &currPos
    );
    
    bool buildRight(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        const int range
    );
    
    bool moveRight(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    
    
    int lookLeft(
        const std::pair<int, int> &currPos
    );
    
    bool buildLeft(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        const int range
    );
    
    bool moveLeft(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    

    int lookUp(
        const std::pair<int, int> &currPos
    );
    
    bool buildUp(
        const std::pair< int, int > &currPos, 
        CoordVector* line, 
        const int range
    );
    
    bool moveUp(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    
    
    int lookDown(
        const std::pair<int, int> &currPos
    );
    
    bool buildDown(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        const int range
    );
    
    bool moveDown(
        const std::pair<int, int> &currPos, 
        CoordVector *line
    );
    
    
    bool extend(
        const std::pair<int, int> &currPos,
        CoordVector *line
    );            
    
    
    bool isVertical(
        const CoordVector *line
    );
    
    bool isHorizontal(
        const CoordVector *line
    );
    
    
    void scanAndLockAroundPos(
        const std::pair<int, int> &currPos
    );
    
    
    void displayLockedCells(
        const unsigned int H, 
        const unsigned int W
    );
    
    void displayLineContainerContents();
    
    void removeDepricatedCells(
        CoordVector *line
    );
    
    void removeDuplicatesInLine(
        CoordVector* line, 
        std::pair< int, int > pos
    );
    
    void addLineToContainer(
        CoordVector* line
    );
    
    void deleteLineFromContainer(
        const int& lineID
    );
    
    int pickLine();
    
public:
    SampleBuilder();
    //SampleBuilder(Dungeon* dungeonInConstruction);
    ~SampleBuilder();
    
    void initNewDungeon(
        int threashold, 
        int heightLimit, 
        int widthLimit
    );
    
    void buildEntrance();
    
    void generateLayout();
    
    void placeEnemies() {};
    
    void placeTraps() {};
    
    void placeLoot() {};
    
    Dungeon* returnResult();
};
