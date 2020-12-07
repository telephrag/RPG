
#include <Dungeon.hh>

Dungeon::Dungeon(unsigned int n, unsigned int m) 
{
	this->layout = new Room** [n];
	
	for ( unsigned int i = 0; i < n; i++ )
		layout[i] = new Room* [m];
	
	for ( unsigned int i = 0; i < n; i++ ) 
	{
		for ( unsigned int j = 0; j < m; j++ )
		{
			layout[i][j] = nullptr;
		}
	}
	
    this->height = n; // The FIRST coordinate, row id
	this->width = m;  // The SECOND coordinate, column id
	// See buildRoom() in SampleBuilder class
}

Dungeon::~Dungeon() {}


unsigned int Dungeon::getEntryX() { return this->entryX; }

unsigned int Dungeon::getEntryY() { return this->entryY; }

unsigned int Dungeon::getHeight() { return this->height; } 

unsigned int Dungeon::getWidth() { return this->width; }

Room*** Dungeon::getLayout() { return this->layout; }

Room* Dungeon::getElem(int y, int x) { return this->layout[x][y]; }

void Dungeon::setEntrance(int y, int x)
{
	this->entryX = x;
	this->entryY = y;
}

void Dungeon::displayMap()
{
	for ( unsigned int i = 0; i < this->height; i++ )
	{
		for ( unsigned int j = 0; j < this->width; j++ )
		{
			if (layout[i][j] != nullptr) 
				std::cout << "[]";
			else 
				std::cout << "--";
		}
		std::cout << std::endl;
	}
}

