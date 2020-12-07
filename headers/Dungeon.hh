
#pragma once
#include "Party.hh"
#include "Room.hh"

class Dungeon
{
private:
	Room* ** layout;
	unsigned int height, width;
	int entryX, entryY;
	
public:
	Dungeon(unsigned int n, unsigned int m);
	~Dungeon();
	
	unsigned int getEntryX(); unsigned int getEntryY();
	unsigned int getHeight(); unsigned int getWidth();
	Room*** getLayout();
	Room* getElem(int x, int y);
	
	void setEntrance(int x = 0, int y = 0);
	void displayMap();

};