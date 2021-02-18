
#include <Dungeon.hh>
#include <iomanip>

Dungeon::Dungeon(unsigned int n, unsigned int m) 
{
    this->layout.reset( new std::unique_ptr< std::unique_ptr<Room>[] >[n]);
    
    for ( unsigned int i = 0; i < n; i++ )
        layout[i].reset( new std::unique_ptr<Room>[m]);
    
    for ( unsigned int i = 0; i < n; i++ ) 
    {
        for ( unsigned int j = 0; j < m; j++ )
        {
            layout[i][j] = nullptr;
        }
    }
    
    this->height = n; // The FIRST coordinate, row id
    this->width = m;  // The SECOND coordinate, column id, x
    // See buildRoom() in SampleBuilder class
}

Dungeon::~Dungeon() {}


unsigned int Dungeon::getEntryX() { return this->entryX; }

unsigned int Dungeon::getEntryY() { return this->entryY; }

unsigned int Dungeon::getHeight() { return this->height; } 

unsigned int Dungeon::getWidth() { return this->width; }

RoomGrid& Dungeon::getLayout() { return layout; }

void Dungeon::setEntrance(int y, int x)
{
    this->entryX = x;
    this->entryY = y;
}

void Dungeon::displayMap()
{
    std::cout << "  ";
    for (unsigned int k = 0; k < this->getWidth(); k++)
    {
        std::cout << std::setw(2);
        std::cout << std::left << k;
    }
    std::cout << "\n";
        
    for ( unsigned int i = 0; i < this->height; i++ )
    {
        std::cout << std::setw(2);
        std::cout << std::left << i;
        
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

