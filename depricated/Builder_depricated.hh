
#pragma once
#include <iostream>
#include <Dungeon.hh>

class Builder
{
public:
    virtual ~Builder() {}
    virtual void initNewDungeon(int threashold, int heightLimit, int widthLimit) = 0;
    virtual void buildEntrance() = 0;
    virtual void buildRoom(int x, int y) = 0;
    virtual void generateLayout() = 0;
    
    virtual void placeEnemies() = 0;
    virtual void placeTraps() = 0;
    virtual void placeLoot() = 0;
};
