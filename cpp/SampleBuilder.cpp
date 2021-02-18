
#include "SampleBuilder.hh"
#include <iomanip>

SampleBuilder::SampleBuilder() 
{
    this->lineContainer = new std::vector<CoordVector>();
}


SampleBuilder::~SampleBuilder() 
{
    std::cout << "Builder has finished his work." << std::endl;
}


void SampleBuilder::initLockedCellsMap()
{
    const unsigned int H = this->dungeon->getHeight();
    const unsigned int W = this->dungeon->getWidth();
    
    this->lockedCells = new bool*[H];
    for (unsigned int i = 0; i < H; i++)
        this->lockedCells[i] = new bool[W];
    
    for (unsigned int i = 0; i < H; i++)
        for (unsigned int j = 0; j < W; j++)
            this->lockedCells[i][j] = false;
}


void SampleBuilder::initNewDungeon(
    int threashold, 
    int heightLimit, 
    int widthLimit
)
{
    int height = (rand() % heightLimit) + threashold;
    int width = (rand() % widthLimit) + threashold;
    Dungeon* temp = new Dungeon(height, width);
    this->dungeon = temp;
    
    initLockedCellsMap();
}


void SampleBuilder::buildEntrance()
{
    int height = this->dungeon->getHeight();
    int width = this->dungeon->getWidth();
    const int entry = rand() % ( (height + width) * 2 );
    int x, y;
    
    if (entry <= width) {
        x = entry; 
        y = 0;
    }
    else if (entry <= height + width) {
        x = width - 1; 
        y = entry - width - 1; 
    }
    else if (entry <= height + width * 2) {
        x = height + width * 2 - entry - 1; 
        y = height - 1; 
    }
    else { 
        x = 0; 
        y = (height + width) * 2 - entry - 1; 
    }
    
    this->dungeon->setEntrance(y, x);
}


void SampleBuilder::buildRoom(
    int y, 
    int x
) 
{
    RoomGrid& temp = this->dungeon->getLayout();
    temp[y][x].reset( new Room() );
    std::cout << "New room was built at: " << y << ", " << x << std::endl; 
}


std::pair<int, int> SampleBuilder::setStartPosAtEntry()
{
    std::pair<int, int> currPos;
    currPos.first = this->dungeon->getEntryY();
    currPos.second = this->dungeon->getEntryX();
    return currPos;
}


void SampleBuilder::buildInitialLine(
    std::pair<int, int> &currPos,
    CoordVector *line
)
{
    this->buildRoom(currPos.first, currPos.second);
    line->push_back(currPos);
    
    if (currPos.second == 0)
        moveRight(currPos, line);
    else if (currPos.second == this->dungeon->getWidth() - 1)
        moveLeft(currPos, line);
    else if (currPos.first == 0)
        moveUp(currPos, line);
    else
        moveDown(currPos, line);
}


void SampleBuilder::deleteLineFromContainer(
    const int &lineID
)
{
    std::vector<CoordVector> *tLineContainer = this->lineContainer;
    
    CoordVector().swap(tLineContainer->at(lineID));
    tLineContainer->erase(tLineContainer->begin() + lineID);
}


std::pair<int, int> SampleBuilder::pickPos(
    const CoordVector* line
)
{
    return line->at( rand() % line->size() );
}


bool SampleBuilder::tryExtend(
    std::pair<int, int> &currPos,
    CoordVector* line,
    int &lineID
)
{
    switch(line->size())
    {
        case 0:
            deleteLineFromContainer(lineID);
        case 1:
        {
            currPos = line->at(0); 
            if (extend(currPos, line)) 
                return true;
            else
            {
                deleteLineFromContainer(lineID);
                return false;
            }
        }
    }
    
    if (line->size() > 1)
    {
        currPos = line->at(0);
        if (extendLong(currPos, line))
            return true;
        else
            return false;
    }
    
    return false;
}


void SampleBuilder::generateLayout()
{
    const unsigned int H = this->dungeon->getHeight();
    const unsigned int W = this->dungeon->getWidth();
    
    std::pair<int, int> currPos = setStartPosAtEntry();
    std::cout << "Entry coords are: " << currPos.first << " " << currPos.second << std::endl;
    
    CoordVector *line = new CoordVector(); //
    
    CoordVector *newLine;

    buildInitialLine(currPos, line);
    addLineToContainer(line);
    
    int seed;
    std::cout << "Please input desired generation seed: "; 
    std::cin >> seed;
    std::cout << "\n";
    
    for (int i = 0; i < 40; i++)
    {
        std::cout << ">> " << i << "\n";
        
        displayLineContainerContents();
        
        srand(seed + i);
        
        int lineID = pickLine(); 
        line = &this->lineContainer->at(lineID); 
        
        if (line->size() > 1) // pickPos()
            currPos = pickPos(line);
        else
        {
            std::cout << "Attempting to extend at coordinate " << currPos.first << ", " << currPos.second << std::endl;
            
            if ( tryExtend(currPos, line, lineID) )
            {
                std::cout << "Extension succesful!" << "\n";
                currPos = pickPos(line);
            }
            else
            {
                std::cout << "Extension failed!" << "\n";
                std::cout << "Skipping iteration\n\n\n";
                continue;
            }   
        }   
        
        std::cout << "Attempting to generate new line at coordinate " 
            << currPos.first << ", " << currPos.second << std::endl;
        
        scanAndLockAroundPos(currPos);
        newLine = new CoordVector(); 
        newLine->push_back(currPos);
        
        if ( isHorizontal(line) )
        {
            moveUp(currPos, newLine);
            moveDown(currPos, newLine);
        }
        else if (isVertical(line))
        {
            moveLeft(currPos, newLine); 
            moveRight(currPos, newLine); 
        }

        removeDepricatedCells(newLine); 
        removeDepricatedCells(line); 
        
        if (newLine->size() >= 2)
            addLineToContainer(newLine);
        
        if (line->size() == 0)
        {
            deleteLineFromContainer(lineID);
        }
        
        this->dungeon->displayMap();
        std::cout << "\n";
        displayLockedCells(H, W);
        std::cout << "\n";
    }
    
    std::cout << "End result:-----------------------------------------------------" << "\n";
    displayLockedCells(H, W);
    
    this->currPos = currPos;
}

/* 
 * <Unimportant>
 * TODO: Using ifdef add killswitch for debuging features.
 * TODO: Consider doing something with unjust cell deprication.
 *        NOTE0: When cell is depricated sometimes algorithm only considers one direction it can be 
 *        built towards. 
 *        Example:
 *        --[]----
 *        --[][][]
 *        --[]----
 *        Here (1, 1) will get depricated although we can build from it to the left to (1, 0).
 *        UPD: (1, 1) is the end of horizontal line. Make algorithm consider line endings when depricating;
 * TODO: Remember why addLineToContainer() is written the way it is.
 * 
 * <Important>
 * TODO: Use noise functions instead of rand();
 * 
 * TODO: 1) Moving to private methods that should be there.
 *       2) Simplify the code wherever possible.
 *       3) Code proper destructor.
 *
 * TODO: Keep debugging this sucker.
 * 
 * <Notes>
 * NOTE: 3 in a block rule is that a square block of 4 cells should have no more than
 *        3 initialized cells. 
 */


void SampleBuilder::removeDuplicatesInLine( // Removes from line coordinates matching given.
    CoordVector *line, 
    std::pair<int, int> pos
)  
{
    for (int i = 0; i < line->size(); i++)
        if (pos == line->at(i))
            line->erase(line->begin() + i);
}


void SampleBuilder::addLineToContainer(
    CoordVector *line
)
{
    std::vector<CoordVector> *tLineContainer = this->lineContainer;
    
    tLineContainer->emplace(tLineContainer->end());
    tLineContainer->at(tLineContainer->size() - 1) = *line;
}


int SampleBuilder::pickLine()
{
    if (this->lineContainer->size() > 0)
        return rand() % this->lineContainer->size();
    else
        throw "Error: No lines to be built upon";
}


int SampleBuilder::randRange(
    const int distance
)
{
    switch (distance) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2: 
            return rand() % 2 + 1;
        default:
            return rand() % (distance - 1) + 1;
    }
}


int SampleBuilder::getFreeRight(
    const std::pair<int, int> &currPos
)
{
    return this->dungeon->getWidth() - currPos.second - 1;
}


int SampleBuilder::getFreeLeft(
    const std::pair<int, int> &currPos
) 
{
    return currPos.second;
}


int SampleBuilder::getFreeUp(
    const std::pair<int, int> &currPos
) 
{
    return this->dungeon->getHeight() - currPos.first - 1;
}


int SampleBuilder::getFreeDown(
    const std::pair<int, int> &currPos
) 
{
    return currPos.first;
}


int SampleBuilder::lookRight( 
    const std::pair<int, int> &currPos
)
{
    const int R = getFreeRight(currPos); 
    if (R == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    int roomsInARow = 0;
    int res = 0;
    
    for (int i = currPos.second + 1; i < this->dungeon->getWidth(); i++)
    {
        if (tLockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    
    return res;
}


bool SampleBuilder::buildRight(
    const std::pair<int, int> &currPos,
    CoordVector *line,
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout(); 
    bool** tLockedCells = this->lockedCells;
    
    for (int i = currPos.second + 1; i < range; i++)
    {
        if (tLockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            this->buildRoom(currPos.first, i);
        }
        
        std::pair<int, int> pos(currPos.first, i);
        scanAndLockAroundPos(pos);
        line->emplace(line->end(), pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveRight(
    const std::pair<int, int> &currPos, 
    CoordVector *line
) 
{
    int R = lookRight(currPos);
    int range = randRange(R) + currPos.second;
    
    if (range > this->dungeon->getWidth() - 1)
        range = this->dungeon->getWidth() - 1;
    
    if (buildRight(currPos, line, range))
        return true;
    else
        return false;
}


int SampleBuilder::lookLeft(
    const std::pair<int, int> &currPos
)
{
    const int L = getFreeLeft(currPos); 
    if (L == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    int roomsInARow = 0;
    int res = 0;
    
    for (int i = currPos.second - 1; i >= 0; i--)
    {
        if (tLockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    
    return res;
}


bool SampleBuilder::buildLeft(
    const std::pair<int, int>& currPos, 
    CoordVector* line, 
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    for (int i = currPos.second - 1; i >= range; i--)
    {
        if (tLockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            this->buildRoom(currPos.first, i);
        }
        
        std::pair<int, int> pos(currPos.first, i);
        scanAndLockAroundPos(pos);
        line->emplace(line->begin(), pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveLeft(
    const std::pair<int, int> &currPos, 
    CoordVector *line
) 
{
    int L = lookLeft(currPos);
    int range = currPos.second - randRange(L);
    
    if (range < 0)
        range = 0;
    
    if (buildLeft(currPos, line, range))
        return true;
    else
        return false;
}


int SampleBuilder::lookUp(
    const std::pair<int, int> &currPos
)
{
    const int U = getFreeUp(currPos);
    if (U == 0)
        return 0;
    
    bool** tLockedCells = this->lockedCells;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    int roomsInARow = 0;
    int res = 0;
    
    for (int j = currPos.first + 1; j < this->dungeon->getHeight(); j++)
    {
        if (tLockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    return res;
}


bool SampleBuilder::buildUp(
    const std::pair<int, int>& currPos, 
    CoordVector* line, 
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    for (int j = currPos.first + 1; j < range; j++)
    {
        if (tLockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            this->buildRoom(j, currPos.second);
        }
        
        std::pair<int, int> pos(j, currPos.second);
        scanAndLockAroundPos(pos);
        line->emplace(line->end(), pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveUp(
    const std::pair<int, int> &currPos, 
    CoordVector *line 
) 
{
    int U = lookUp(currPos); 
    int range = currPos.first + randRange(U);
    
    if (range > this->dungeon->getHeight() - 1)
        range = this->dungeon->getHeight() - 1;    
    
    if (buildUp(currPos, line, range))
        return true;
    else
        return false;
}


int SampleBuilder::lookDown(
    const std::pair<int, int> &currPos
)
{
    const int D = getFreeDown(currPos);
    if (D == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    int roomsInARow = 0;
    int res = 0;
    
    for (int j = currPos.first - 1; j >= 0; j--)
    {
        if (tLockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    return res;
}


bool SampleBuilder::buildDown(
    const std::pair<int, int>& currPos, 
    CoordVector* line, 
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    for (int j = currPos.first - 1; j >= range; j--)
    {
        if (tLockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            this->buildRoom(j, currPos.second);
        }
            
        std::pair<int, int> pos(j, currPos.second);
        scanAndLockAroundPos(pos);
        line->emplace(line->begin(), pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveDown(
    const std::pair<int, int> &currPos, 
    CoordVector *line
) 
{
    int D = lookDown(currPos);
    int range = currPos.first - randRange(D);
    
    if (range < 0)
        range = 0;
    
    if (buildDown(currPos, line, range))
        return true;
    else
        return false;
}


bool SampleBuilder::extend(
    const std::pair<int, int> &currPos, 
    CoordVector *line
)
{
    const int R = getFreeRight(currPos);
    const int L = getFreeLeft(currPos);
    const int U = getFreeUp(currPos);
    const int D = getFreeDown(currPos);
    
    bool right = false, left = false, up = false, down = false;
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    
    if (R != 0 || L != 0)
    {
        if (R != 0 && tLayout[currPos.first][currPos.second + 1] == nullptr)
            right = moveRight(currPos, line);
        if (L != 0 && tLayout[currPos.first][currPos.second - 1] == nullptr)
            left = moveLeft(currPos, line);
    }
    else
    {
        if (U != 0 && tLayout[currPos.first + 1][currPos.second] == nullptr)
            up = moveUp(currPos, line); 
        if (D != 0 && tLayout[currPos.first - 1][currPos.second] == nullptr)
            down = moveDown(currPos, line);
    }
    
    if ( (right && left) || (up && down) )
        tLockedCells[currPos.first][currPos.second] = true;
        
    if (right || left || up || down)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool SampleBuilder::extendLong(
    std::pair<int, int>& currPos, 
    CoordVector* line
)
{
    if (line->size() < 2)
    {
        throw "Recieved line is not long (has length 2 or more). Use extend().";
    }
    
    bool right = false, left = false, up = false, down = false;
    
    if ( isHorizontal(line) ) 
    {
        up = moveUp(currPos, line);
        if (up)
            line->erase(line->begin());
        down = moveDown(currPos, line);
    }
    else
    {
        left = moveLeft(currPos, line);
        if (left)
            line->erase(line->begin());
        right = moveRight(currPos, line); 
    }   
    
    if (right || left || up || down)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool SampleBuilder::isHorizontal(
    const CoordVector *line
)
{
    if (line->size() < 2)
    {
        std::cout << "Line's length is less than 2." << std::endl;
        return false;
    }
    
    if (line->at(0).second != line->at(1).second)
        return true;
    return false;
}


bool SampleBuilder::isVertical(
    const CoordVector *line
)
{
    if (line->size() < 2)
    {
        std::cout << "Line's length is less than 2. " << std::endl;
        return false;
    }
    if (line->at(0).first != line->at(1).first)
        return true;
    return false;
}


void SampleBuilder::scanAndLockAroundPos(
    const std::pair<int, int> &currPos 
)
{
    const int R = getFreeRight(currPos);
    const int L = getFreeLeft(currPos);
    const int U = getFreeUp(currPos);
    const int D = getFreeDown(currPos);
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    bool** tLockedCells = this->lockedCells;
    int globalCount = 0;
    
    if (R != 0)
    {
        if (U != 0)
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first + 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second };
                
            if (tLayout[currPos.first + 1][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second + 1 };
                
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (tLayout[currPos.first][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second + 1 };
                
            if (count == 3)
            {
                tLockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
        
        if ( D != 0 )
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (tLayout[currPos.first][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second + 1 };
                
            if (tLayout[currPos.first - 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second };
                
            if (tLayout[currPos.first - 1][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second + 1 };
                
            if (count == 3)
            {
                tLockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
    }
    
    if (L != 0)
    {
        if (U != 0)
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first + 1][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second - 1 };
                
            if (tLayout[currPos.first + 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second };
                
            if (tLayout[currPos.first][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second - 1 };
                
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (count == 3)
            {
                tLockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
        
        if ( D != 0 )
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second - 1 };
                
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (tLayout[currPos.first - 1][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second - 1 };
                
            if (tLayout[currPos.first - 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second };
                
            if (count == 3)
            {
                tLockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
    }
    
    if (globalCount == 4)
    {
        tLockedCells[currPos.first][currPos.second] = true;
    }
}


void SampleBuilder::displayLockedCells(
    const unsigned int H, 
    const unsigned int W
)
{
    bool** tLockedCells = this->lockedCells;
    
    std::cout << "  ";
    for (unsigned int k = 0; k < this->dungeon->getWidth(); k++)
    {
        std::cout << std::setw(2);
        std::cout << std::left << k;
    }
    std::cout << "\n";
    
    for (unsigned int i = 0; i < H; i++)
    {
        std::cout << std::setw(2);
        std::cout << std::left << i;
        
        for (unsigned int j = 0; j < W; j++)
            if (tLockedCells[i][j])
                std::cout << "()";
            else
                std::cout << "--";
                
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << std::endl;
}


void SampleBuilder::displayLineContainerContents()
{
    std::vector<CoordVector> *tLineContainer = this->lineContainer;
    
    for (int i = 0; i < tLineContainer->size(); i++)
    {
        std::cout << i << ": ";
        for (int j = 0; j < tLineContainer->at(i).size(); j++)
            std::cout << "(" << tLineContainer->at(i).at(j).first << ";" << tLineContainer->at(i).at(j).second << ") ";
        std::cout << "\n";
    }
}





void SampleBuilder::removeDepricatedCells(
    CoordVector *line
)
{
    bool** tLockedCells = this->lockedCells;
    
    for (unsigned int i = 0; i < line->size(); i++)
    {
        if (tLockedCells[line->at(i).first][line->at(i).second])
            line->erase(line->begin() + i);
    }
}


Dungeon* SampleBuilder::returnResult() { return this->dungeon; }

