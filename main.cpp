
#include <iostream>
#include <vector>
#include "headers/Dungeon.hh"
#include "headers/DungeonMaster.hh"
#include "headers/SampleBuilder.hh"
#include "headers/Exterrnals.hh" // TODO: figure out wether you need this header

// TODO: code class resposible for dungeons, battles and enemies generation 

// TODO: code class Printer that shows enemy's and player's party in a single row

int main() {
    
    DungeonMaster* master = new DungeonMaster();
    SampleBuilder* builder = new SampleBuilder();
    
    master->setBuilder(builder);
    master->makeSampleDungeon();
    
    Dungeon* dungeon = builder->returnResult();
    
    dungeon->displayMap();
    
    delete master;
    delete builder;
    
    std::cin.ignore();
    std::cin.get();
    
    return 0;
}   
