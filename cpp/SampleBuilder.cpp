
#include "SampleBuilder.hh"

SampleBuilder::SampleBuilder() {}

SampleBuilder::SampleBuilder(Dungeon* dungeonInConstruction)
{
	this->dungeon = dungeonInConstruction;
}

SampleBuilder::~SampleBuilder() 
{
	std::cout << "Builder has finished his work." << std::endl;
}

void SampleBuilder::initNewDungeon(int threashold, int heightLimit, int widthLimit)
{
	int height = (rand() % heightLimit) + threashold;
	int width = (rand() % widthLimit) + threashold;
	Dungeon* temp = new Dungeon(height, width);
	this->dungeon = temp;
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
	// this->buildRoom(y, x);
}

void SampleBuilder::buildRoom(int y, int x) // Move to private later!
{
	Room*** temp = this->dungeon->getLayout();
	temp[y][x] = new Room();
	std::cout << "New room was built at: " << y << ", " << x << std::endl;
}

void SampleBuilder::generateLayout()
{
	std::pair<int, int> currPos;
	currPos.first = this->dungeon->getEntryY();
	currPos.second = this->dungeon->getEntryX();
    std::cout << "Entry coords are: " << currPos.first << " " << currPos.second << std::endl;
	
	
    const unsigned int H = this->dungeon->getHeight();
	const unsigned int W = this->dungeon->getWidth();
	
	bool** lockedCells = new bool*[H];
	for (unsigned int i = 0; i < H; i++)
		lockedCells[i] = new bool[W];
	
	for (unsigned int i = 0; i < H; i++)
		for (unsigned int j = 0; j < W; j++)
			lockedCells[i][j] = false;
			
	
    std::vector<std::pair<int, int>> *line = new std::vector<std::pair<int, int>>();
    std::vector<std::pair<int, int>> *newLine = new std::vector<std::pair<int, int>>();
    std::vector< std::vector<std::pair<int, int>> > lineContainer;

    if (currPos.second == 0)
        moveRight(currPos, line, lockedCells);
    else if (currPos.second == W - 1)
        moveLeft(currPos, line, lockedCells);
	else if (currPos.first == 0)
		moveUp(currPos, line, lockedCells);
	else
		moveDown(currPos, line, lockedCells);
	
	lineContainer.push_back(*line);
	
    
	for (int i = 0; i < 20; i++)
	{
		srand(i + i);
        int lineID;
        if (lineContainer.size() != 0)
            lineID = rand() % lineContainer.size();
        else
        {
            std::cout << "Generation ended prematurely, no lines left to build upon. \n";
            break;
        }   
		line = &lineContainer.at(lineID); 
        
        if (line->size() > 1)
            currPos = line->at( rand() % line->size() );
        else
        {
            int size = line->size();
            switch(size)
            {
                case 0:
                    std::vector<std::pair<int, int>>().swap(lineContainer.at(lineID));
                    lineContainer.erase(lineContainer.begin() + lineID);
                case 1:
                    currPos = line->at(0); // create findPosToExtendFrom
                    if (extend(currPos, line, lockedCells)) 
                        currPos = line->at(rand() % line->size());
                    else
                    {
                        std::vector<std::pair<int, int>>().swap(lineContainer.at(lineID));
                        lineContainer.erase(lineContainer.begin() + lineID);
                        std::cout << "Skipping iteration of loop at number " << i << "\n\n\n";
                        continue;
                    }
            }
        }
		
		if ( isHorizontal(line) )
		{
			scanAndLockAroundPos(lockedCells, currPos, this->dungeon->getLayout());
			
			newLine = new std::vector<std::pair<int, int>>();
			bool up = moveUp(currPos, newLine, lockedCells);
			if (newLine->size() > 0)
				newLine->erase(newLine->begin());
            bool down = moveDown(currPos, newLine, lockedCells);

			if (!up && !down)
            {
				lockedCells[currPos.first][currPos.second] = true;
                std::cout << "Cell is locked at coordinate " << currPos.first << ", " << currPos.second << std::endl;
            }
            
			scanAndLockAroundPos(lockedCells, currPos, this->dungeon->getLayout());
            
            std::cout << "Attempted to generate new line at loop number " << i << " at coordinate " << currPos.first << ", " << currPos.second << std::endl;
		}
		else if (isVertical(line))
		{
			scanAndLockAroundPos(lockedCells, currPos, this->dungeon->getLayout());
			
			newLine = new std::vector<std::pair<int, int>>();
			bool left = moveLeft(currPos, newLine, lockedCells);
			if (newLine->size() > 0)
				newLine->erase(newLine->begin());
			bool right = moveRight(currPos, newLine, lockedCells); 
			
			if (!left && !right)
            {
				lockedCells[currPos.first][currPos.second] = true;
                std::cout << "Cell is locked at coordinate " << currPos.first << ", " << currPos.second << std::endl;
            }
            
			scanAndLockAroundPos(lockedCells, currPos, this->dungeon->getLayout());
            
            std::cout << "Attempted to generate new line at loop number " << i << " at coordinate " << currPos.first << ", " << currPos.second << std::endl;
		}
		
		removeDepricatedCells(newLine, lockedCells); 
		removeDepricatedCells(line, lockedCells); 
        /* 
         * Perhaps I should iterate over not only line and newLine but all of the lines in      
         * lineContainer Because there can be intersections with 
         * other lines at depricated points. 
         */
		
		if (newLine->size() != 0)
			addLineToContainer(lineContainer, newLine);
        
        if (line->size() == 0)
        {
            std::vector<std::pair<int, int>>().swap(lineContainer.at(lineID)); // replace with deleteLineFromContainer() ?
            lineContainer.erase(lineContainer.begin() + lineID);
        }
		
		this->dungeon->displayMap();
		std::cout << std::endl;
		displayLockedCellsDB(lockedCells, H, W);
		std::cout << std::endl;
	}
	std::cout << "End result:-----------------------------------------------------" << std::endl;
	displayLockedCellsDB(lockedCells, H, W);
	
/* 
 * <Unimportant>
 * TODO3: Add srand() to everywhere rand() is used.
 * TODO4: Using ifdef add killswitch for debuging features.
 * TODO5: Consider doing something with unjust cell deprication.
 *        NOTE0: When cell is depricated sometimes algorithm only considers one direction it can be 
 *        built towards. 
 *        Example:
 *        --[]----
 *        --[][][]
 *        --[]----
 *        Here (1, 1) will get depricated although we can build from it to the left to (1, 0).
 *
 * TODO: Refactoring
 *       1) Moving to private methods that should be there.
 *       2) Transitioning to std::array< std::array<std::unique_ptr<Room>> > 
 *          for storage inside Dungeon.
 *       3) Switch to \n from std::endl.
 *       4) Simplify the code where possible.
 *       5) Transition to using std::multimapmap<int, int> from std::vector<std::pair<int, int>>.
 *       6) Code proper destructor.
 * 
 * <Important>
 * TODO7: In move methods make use of emplace so that cells coordinates are at correct order.
 * 
 * TODO: Keep debugging this sucker.
 * 
 * <Notes>
 * NOTE0: 3 in a block rule is that a square block of 4 cells should have no more than
 *        3 initialized cells. 
 */
}

bool SampleBuilder::isInLine(std::vector<std::pair<int, int>> *line, std::pair<int, int> pos)
{
    for (int i = 0; i < line->size(); i++)
        if (pos == line->at(i))
            line->erase(line->begin() + i);
}

void SampleBuilder::addLineToContainer(std::vector< std::vector<std::pair<int, int>> > &container, 
									   std::vector<std::pair<int, int>> *line)
{
	container.emplace(container.end());
	container.at(container.size() - 1) = *line;
}

std::vector<std::pair<int, int>> SampleBuilder::pickLine(
	const std::vector< std::vector<std::pair<int, int>> > &lineContainer)
{
	if (lineContainer.size() > 0)
		return lineContainer.at( rand() % lineContainer.size() );
	else
		throw "Cells has no lines to be built upon";
}

bool SampleBuilder::moveRight(const std::pair<int, int> &currPos, 
                              std::vector<std::pair<int, int>> *line,
							  bool** lockedCells)
{
	const int R = this->dungeon->getWidth() - currPos.second - 1; // amount of free lineContainer to right
	if (R == 0)
		return false;
	
	int range;
	if (R == 1)
		range = 1;
	else
		range = rand() % (R - 1) + 1;
		
	Room*** tLayout = this->dungeon->getLayout();
	int roomsInARow = 0;
    
    isInLine(line, currPos);
	
	for (int i = currPos.second; i < range + currPos.second; i++)
	{
		if (lockedCells[currPos.first][i])
			break;
		
		if (tLayout[currPos.first][i] == nullptr)
		{
			this->buildRoom(currPos.first, i);
			roomsInARow = 0;
		}
		else
		{
			roomsInARow++;
			if (roomsInARow > 1)
				break;
		}
		
		std::pair<int, int> pos(currPos.first, i);
		scanAndLockAroundPos(lockedCells, pos, tLayout);
		line->push_back(pos);
	}
	
	if (line->size() < 2)
		return false;
	else
		return true;
}

bool SampleBuilder::moveLeft(const std::pair<int, int> &currPos, 
							 std::vector<std::pair<int, int>> *line, 
							 bool** lockedCells) 
{
	const int L = currPos.second;
	if (L == 0)
		return false;
	
	int range;
	if (L == 1)
		range = 1;
	else
		range = rand() % (L - 1) + 1;
		
	Room*** tLayout = this->dungeon->getLayout();
	int roomsInARow = 0;
    
    isInLine(line, currPos);
	
	for (int i = currPos.second; i >= currPos.second - range; i--)
	{
		if (lockedCells[currPos.first][i])
			break;
		
		if (tLayout[currPos.first][i] == nullptr)
		{
			this->buildRoom(currPos.first, i);
			roomsInARow = 0;
		}
		else
		{
			roomsInARow++;
			if (roomsInARow > 1)
				break;
		}
		
		std::pair<int, int> pos(currPos.first, i);
		scanAndLockAroundPos(lockedCells, pos, tLayout);
		line->push_back(pos);
	}
	
	if (line->size() < 2)
		return false;
	else
		return true;
}

bool SampleBuilder::moveUp(const std::pair<int, int> &currPos, 
                           std::vector<std::pair<int, int>> *line,
						   bool** lockedCells) 
{
	const int U = this->dungeon->getHeight() - currPos.first - 1;
	if (U == 0)
		return false;
	
	int range;
	if (U == 1)
		range = 1;
	else
		range = rand() % (U - 1) + 1;
		
	Room*** tLayout = this->dungeon->getLayout();
	int roomsInARow = 0;
    
    isInLine(line, currPos);
    
	for (int j = currPos.first; j <= currPos.first + range; j++)
	{
		if (lockedCells[j][currPos.second])
			break;
		
		if (tLayout[j][currPos.second] == nullptr)
		{
			this->buildRoom(j, currPos.second);
			roomsInARow = 0;
		}
		else
		{
			roomsInARow++;
			if (roomsInARow > 1)
				break;
		}
		
		std::pair<int, int> pos(j, currPos.second);
		scanAndLockAroundPos(lockedCells, pos, tLayout);
        line->push_back(pos);
	}
	
	if (line->size() < 2)
		return false;
	else
		return true;
}

bool SampleBuilder::moveDown(const std::pair<int, int> &currPos, 
                             std::vector<std::pair<int, int>> *line,
							 bool** lockedCells) 
{
	const int D = currPos.first;
	if (D == 0)
		return false;
	
	int range;
	if (D == 1)
		range = 1;
	else
		range = rand() % (D - 1) + 1;
		
	Room*** tLayout = this->dungeon->getLayout();
	int roomsInARow = 0;
    
    isInLine(line, currPos);
    
	for (int j = currPos.first; j >= currPos.first - range; j--)
	{
		if (lockedCells[j][currPos.second])
			break;
		
		if (tLayout[j][currPos.second] == nullptr)
		{
			this->buildRoom(j, currPos.second);
			roomsInARow = 0;
		}
		else
		{
			roomsInARow++;
			if (roomsInARow > 1)
				break;
		}
			
		std::pair<int, int> pos(j, currPos.second);
		scanAndLockAroundPos(lockedCells, pos, tLayout);
		line->push_back(pos);
	}
	
	if (line->size() < 2)
		return false;
	else
		return true;
}

bool SampleBuilder::extend(const std::pair<int, int> &currPos,
						   std::vector<std::pair<int, int>> *line,
						   bool** lockedCells)
{
    std::cout << "Attempting to extend at coordinate " << currPos.first << ", " << currPos.second << std::endl;
    
    const int R = this->dungeon->getWidth() - currPos.second - 1;
	const int L = currPos.second;
	const int U = this->dungeon->getHeight() - currPos.first - 1;
	const int D = currPos.first;
    
    bool right = false, left = false, up = false, down = false;
    // create copy of line
    Room*** tLayout = this->dungeon->getLayout();
    
    if (R != 0 || L != 0)
    {
        if (R != 0 && tLayout[currPos.first][currPos.second + 1] == nullptr)
            right = moveRight(currPos, line, lockedCells);
        if (L != 0 && tLayout[currPos.first][currPos.second - 1] == nullptr)
            left = moveLeft(currPos, line, lockedCells);
    }
    else
    {
        if (U != 0 && tLayout[currPos.first + 1][currPos.second] == nullptr)
            up = moveUp(currPos, line, lockedCells); 
        if (D != 0 && tLayout[currPos.first - 1][currPos.second] == nullptr)
            down = moveDown(currPos, line, lockedCells);
    }
    
    if ( (right && left) || (up && down) )
        lockedCells[currPos.first][currPos.second] = true;
        
    if (right || left || up || down)
    {
        std::cout << "Extension succesful!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Extension failed!" << std::endl;
        return false;
    }
}

bool SampleBuilder::isHorizontal(const std::vector<std::pair<int, int>> *line)
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

bool SampleBuilder::isVertical(const std::vector<std::pair<int, int>> *line)
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

void SampleBuilder::scanAndLockAroundPos(bool** lockedCells, 
										 const std::pair<int, int> &currPos,
										 Room*** tLayout)
{
	const int R = this->dungeon->getWidth() - currPos.second - 1;
	const int L = currPos.second;
	const int U = this->dungeon->getHeight() - currPos.first - 1;
	const int D = currPos.first;
    
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
				lockedCells[empty.first][empty.second] = true;
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
				lockedCells[empty.first][empty.second] = true;
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
				lockedCells[empty.first][empty.second] = true;
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
				lockedCells[empty.first][empty.second] = true;
                globalCount++;
			}
		}
	}
	
	if (globalCount == 4)
    {
        lockedCells[currPos.first][currPos.second] = true;
        //lockCellsHorizontalToPos(lockedCells, currPos);
        //lockCellsVerticalToPos(lockedCells, currPos);
    }
}

void SampleBuilder::lockCellsDiagonalToPos(bool** lockedCells, const std::pair<int, int> &currPos)
{
	const int R = this->dungeon->getWidth() - currPos.second - 1;
	const int L = currPos.second;
	const int U = this->dungeon->getHeight() - currPos.first - 1;
	const int D = currPos.first;
	
	if (R != 0)
	{
		if (U != 0)
			lockedCells[currPos.first + 1][currPos.second + 1] = true;
		if (D != 0)
			lockedCells[currPos.first - 1][currPos.second + 1] = true;
	}
	
	if (L != 0)
	{
		if (U != 0)
			lockedCells[currPos.first + 1][currPos.second - 1] = true;
		if (D != 0)
			lockedCells[currPos.first - 1][currPos.second - 1] = true;
	}
	
	lockedCells[currPos.first][currPos.second] = true;
}

void SampleBuilder::lockCellsHorizontalToPos(bool** lockedCells, const std::pair<int, int> &currPos)
{
	const int R = this->dungeon->getWidth() - currPos.second - 1;
	const int L = currPos.second;
	
	if (R != 0)
	{
		lockedCells[currPos.first][currPos.second + 1] = true;
	}
	
	if (L != 0)
	{
		lockedCells[currPos.first][currPos.second - 1] = true;
	}
}

void SampleBuilder::lockCellsVerticalToPos(bool** lockedCells, const std::pair<int, int> &currPos)
{
	const int U = this->dungeon->getHeight() - currPos.first - 1;
	const int D = currPos.first;
	
	if (U != 0)
		lockedCells[currPos.first + 1][currPos.second] = true;
	
	if (D != 0)
		lockedCells[currPos.first - 1][currPos.second] = true;
}

void SampleBuilder::displayLockedCellsDB(bool** lockedCells, 
										 const unsigned int H, 
										 const unsigned int W)
{
		for (unsigned int i = 0; i < H; i++)
		{
			for (unsigned int j = 0; j < W; j++)
				if (lockedCells[i][j])
					std::cout << "L";
				else
					std::cout << "-";
					
			std::cout << std::endl;
		}
		
		std::cout << std::endl;
		std::cout << std::endl;
}

void SampleBuilder::removeDepricatedCells(std::vector<std::pair<int, int>> *line, bool** lockedCells)
{
	for (unsigned int i = 0; i < line->size(); i++)
	{
		if (lockedCells[line->at(i).first][line->at(i).second])
			line->erase(line->begin() + i);
	}
}

Dungeon* SampleBuilder::returnResult() { return this->dungeon; }
