
#include <SampleBuilder.hh>
#include <iomanip>


SampleBuilder::SampleBuilder()
{
    this->lineContainer = new std::vector<CoordVector>();
}


SampleBuilder::~SampleBuilder()
{
    std::cout << "Builder finished work. \n";
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
 * TODO: 2) Simplify the code wherever possible.
 *       3) Code proper destructor.
 *
 * TODO: Keep debugging this sucker.
 * 
 * <Notes>
 * NOTE: 3 in a block rule is that a square block of 4 cells should have no more than
 *        3 initialized cells. 
 */

void SampleBuilder::placeEnemies() {} // CODEME

void SampleBuilder::placeTraps() {}

void SampleBuilder::placeLoot() {}

