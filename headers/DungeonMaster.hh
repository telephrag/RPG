
#pragma once
#include <Builder.hh>

class DungeonMaster
{
private:
	Builder* builder;
	
public:
	~DungeonMaster();
	DungeonMaster();
	void setBuilder(Builder* newBuilder);
	void makeSampleDungeon();
	
};