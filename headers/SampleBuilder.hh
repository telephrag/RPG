
#pragma once
#include<iostream>
#include<Builder.hh>

class SampleBuilder: public Builder
{   
private:
    Dungeon* dungeon;
    
    bool moveRight(const std::pair<int, int> &currPos, 
                   std::vector<std::pair<int, int>> *line,
                   bool** lockedCells);
    
    bool moveLeft(const std::pair<int, int> &currPos, 
                  std::vector<std::pair<int, int>> *line, 
                  bool** lockedCells);
    
    bool moveUp(const std::pair<int, int> &currPos, 
                std::vector<std::pair<int, int>> *line,
                bool** lockedCells);
    
    bool moveDown(const std::pair<int, int> &currPos, 
                  std::vector<std::pair<int, int>> *line,
                  bool** lockedCells);
    
    bool extend(const std::pair<int, int> &currPos,
                           std::vector<std::pair<int, int>> *line,
                           bool** lockedCells);            
    
    bool isVertical(const std::vector<std::pair<int, int>> *line);
    
    bool isHorizontal(const std::vector<std::pair<int, int>> *line);
    
    void lockCellsDiagonalToPos(bool** lockedCells, const std::pair<int, int> &currPos);
    void lockCellsHorizontalToPos(bool** lockedCells, const std::pair<int, int> &currPos);
    void lockCellsVerticalToPos(bool** lockedCells, const std::pair<int, int> &currPos);
    void scanAndLockAroundPos(bool** lockedCells, 
                              const std::pair<int, int> &currPos, 
                              Room*** tLayout);
    
    void displayLockedCellsDB(bool** lockedCells, const unsigned int H, const unsigned int W);
    
    void removeDepricatedCells(std::vector<std::pair<int, int>> *line, bool** lockedCells);
    
    bool isInLine(std::vector<std::pair<int, int>> *line, std::pair<int, int> pos);
    
    void addLineToContainer(std::vector< std::vector<std::pair<int, int>> > &container, 
                            std::vector<std::pair<int, int>> *line);
    
    std::vector<std::pair<int,int>> pickLine(
        const std::vector< std::vector<std::pair<int, int>> > &lineContainer);
    
public:
    SampleBuilder();
    SampleBuilder(Dungeon* dungeonInConstruction);
    ~SampleBuilder();
    
    void initNewDungeon(int threashold, int heightLimit, int widthLimit);
    void buildEntrance();
    void buildRoom(int x, int y);
    void generateLayout();
    
    void placeEnemies() {};
    void placeTraps() {};
    void placeLoot() {};
    
    Dungeon* returnResult();
};
