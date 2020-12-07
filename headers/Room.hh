
#pragma once
#include <Party.hh>
#include <Loot.hh>
#include <variant>

using CType = std::variant<Loot, Party>;
using HVector = std::vector<CType>;

class Room
{
public:
	
	Room();
	~Room();
	void sortContByPriority();
	
private:
	HVector content;
};

