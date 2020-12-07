
#include <DungeonMaster.hh>

DungeonMaster::DungeonMaster() {}

DungeonMaster::~DungeonMaster()
{
	std::cout << "Dungeon-master has left the dungeon." << std::endl;
}

void DungeonMaster::makeSampleDungeon()
{
	this->builder->initNewDungeon(5, 10, 10);
	this->builder->buildEntrance();
//	this->builder->buildRoom(0, 0);
//	this->builder->buildRoom(0, 1);
//	this->builder->buildRoom(0, 2);
//	this->builder->buildRoom(0, 3);
//	this->builder->buildRoom(1, 3);
//	this->builder->buildRoom(2, 3);
	
	this->builder->generateLayout();
	this->builder->placeEnemies();
	this->builder->placeTraps();
	this->builder->placeLoot();
}

void DungeonMaster::setBuilder(Builder* newBuilder)
{
	this->builder = newBuilder;
}