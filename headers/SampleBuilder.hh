#pragma once
#include <Builder.hh>

class SampleBuilder : public Builder
{
public:
    SampleBuilder();
    ~SampleBuilder();
    
    virtual void initNewDungeon(
        int threashold, 
        int heightLimit, 
        int widthLimit
    );
    
    virtual void buildEntrance();
    
    virtual void generateLayout();
    
    virtual void placeEnemies();
    
    virtual void placeTraps();
    
    virtual void placeLoot();
};
