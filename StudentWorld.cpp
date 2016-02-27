#include "StudentWorld.h"
#include <iostream> //for debugging
#include <string>
#include <algorithm> //for remove_if
#include <cmath> //for square root
#include <random> //for random numbers
#include <queue> //for determining a protester's exit point

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
  return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
  : GameWorld(assetDir)
{}

StudentWorld::~StudentWorld()
{
  //Just call the clean up function, which already deals
  //with any dynamically allocated memory that must be
  //handled
  cleanUp();
}

int StudentWorld::init()
{
  //Set number of barrels currently on oil field
  m_barrels = 0;

  //TODO: Add actors randomly
  //Dummy implementation: just some stuff directly
  m_actors.push_back(new Boulder(10, 20, this));
  m_actors.push_back(new Boulder(20, 30, this));
  m_actors.push_back(new Boulder(30, 40, this));
  m_actors.push_back(new Boulder(10, 50, this));
  m_actors.push_back(new WaterPool(15, 20, this));
  m_actors.push_back(new WaterPool(24, 35, this));
  m_actors.push_back(new OilBarrel(40, 45, this));
  m_actors.push_back(new OilBarrel(35, 50, this));
  m_actors.push_back(new OilBarrel(45, 55, this));
  //m_actors.push_back(new RegularProtester(this));
  m_actors.push_back(new HardCoreProtester(this));
  m_actors.push_back(new SonarKit(2, 55, this));
  m_actors.push_back(new GoldNugget(55, 55, this, false, true, true));
  m_actors.push_back(new GoldNugget(5, 5, this, false, true, true));

  //Add all the dirt
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      m_dirt[i][k] = new Dirt(i, k, this);
    }
  }
  //Delete all unnecessary dirt for initial mineshaft
  for(int i = 30; i <= 33; i++){
    for(int k = 4; k <= 59; k++){
      delete m_dirt[i][k]; 
      m_dirt[i][k] = NULL;
    }
  }

  //Allocate and insert FrackMan object into game
  m_frackman = new FrackMan(this);

  return GWSTATUS_CONTINUE_GAME;
}

//Determines if pointed to Actor is dead. If so, deletes and returns true
bool isDead(Actor* p)
{
  if (!p->isAlive()){
    delete p;
    return true;
  }
  return false;
}

int StudentWorld::move()
{
  //Update the text with at the top with score/level/lives etc.
  setDisplayText();

  //Ask all actors to do something
  m_frackman->doSomething();
  for(size_t i = 0; i < m_actors.size(); i++){
    m_actors[i]->doSomething();
  }

  //Check if the FracKMan has been killed
  if( m_frackman->getHealth() <= 0 ){
    //Decrement the number of lives
    decLives();

    //Return that the player has died
    return GWSTATUS_PLAYER_DIED;
  }

  //Check if all the oil barrels have been collected
  if(m_barrels <= 0){
    //Play sound
    playSound(SOUND_FINISHED_LEVEL);

    //Move onto next level
    return GWSTATUS_FINISHED_LEVEL;
  }

  //Delete all actors that are dead
  m_actors.erase(remove_if(m_actors.begin(), m_actors.end(), isDead), m_actors.end());

  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
  //Delete the FrackMan
  delete m_frackman;

  //Find all the dirt that still exists and delete it, setting
  //the pointer at that spot in the array to null
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      if(m_dirt[i][k] != NULL){
	delete m_dirt[i][k];
	m_dirt[i][k] = NULL;
      }
    }
  }


  int origSize = m_actors.size();
  //Delete all dynamically allocated actors
  for(size_t i = 0; i < origSize; i++){
    //Free up the memory
    delete m_actors[0];

    //Remove from the vector
    m_actors.erase( m_actors.begin() );
  }

  return; 
}

//Searches for dirt at the location (x, y)
//if it is found, then the dirt is deleted
bool StudentWorld::destroyDirt(int x, int y)
{
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      //Do not hceck spots in the array with null pointers
      if (m_dirt[i][k] == NULL)
	continue;

      if ( m_dirt[i][k]->getX() == x && m_dirt[i][k]->getY() == y ){
	delete m_dirt[i][k];
	m_dirt[i][k] = NULL;
	return true;
      }
    }
  }
  return false;
}

bool StudentWorld::isDirt(int x, int y) const
{
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      //Do not check null dirt
      if(m_dirt[i][k] == NULL)
	continue;

      if(m_dirt[i][k]->getX() == x && m_dirt[i][k]->getY() == y)
	return true;
    }
  }
  return false;
}

//Returns a pointer to an actor at position (x, y)
//If there is no actor there, then return NULL
Actor* StudentWorld::getActor(int x, int y)
{
  for(size_t i = 0; i < m_actors.size(); i++){
    if(m_actors[i]->getX() == x && m_actors[i]->getY() == y){
      return m_actors[i];
    }
  }
  return NULL;
}

//Allows other classes to give actors to the StudentWorld class
//to manage
void StudentWorld::giveActor(Actor* p)
{
  m_actors.push_back(p);
}

FrackMan* StudentWorld::findNearbyFrackMan(Actor* a, int radius) const
{
  int thix = a->getX();
  int thiy = a->getY();

  int frackx = m_frackman->getX();
  int fracky = m_frackman->getY();

  double distanceSquared = (double) ((thix - frackx) * (thix - frackx)) + ((thiy-fracky) * (thiy - fracky));
  double distance = sqrt(distanceSquared);

  if(distance <= (double) radius)
    return m_frackman;

  return NULL;
}

Protester* StudentWorld::findNearbyProtester(Actor* a, int radius) const
{
  for(size_t i = 0; i < m_actors.size(); i++){
    //check that actor being examined is a protester
    if(m_actors[i]->isProtester()){
      //check that actor being examined is within the radius given
      if(withinRadiusOf(a->getX(), a->getY(), m_actors[i]->getX(), m_actors[i]->getY(), radius))
	return (RegularProtester*) m_actors[i];
    }
  }
  return NULL;
}

//annoy all protesters within a certain radius by the given amount
//if at least one protester is annoyed, return true
//otherwise, return false
bool StudentWorld::annoyNearbyProtesters(Actor* a, int radius, int amount)
{
  int x = a->getX();
  int y = a->getY();

  bool flag = false;

  for(size_t i = 0; i < m_actors.size(); i++){
    Actor* t = m_actors[i];
    //Test if actor is within the proper radius
    if( withinRadiusOf(x, y, t->getX(), t->getY(), radius) ){
      //Test if the actor being examined is a protester
      if( t->isProtester() ){
	//Annoy the protester
	t->annoyAgent(amount);
	flag = true;
      }
    }
  }
  return flag;
}

void StudentWorld::giveFrackManWater(int count)
{
  int curWater = m_frackman->getWater();
  m_frackman->setWater(curWater + count);
}

void StudentWorld::giveFrackManSonars(int count)
{
  int curSonars = m_frackman->getSonars();
  m_frackman->setSonars(curSonars + count);
}

int StudentWorld::addBarrels(int num)
{
  m_barrels += num;
}

void StudentWorld::useSonar(FrackMan* p)
{
  int xCoord = p->getX();
  int yCoord = p->getY();

  //For each member of array of Actors, call function to make it visible
  //if it is within 12 distance from the FrackMan
  for(size_t i = 0; i < m_actors.size(); i++){
    int actorX = m_actors[i]->getX();
    int actorY = m_actors[i]->getY();

    int distanceSquared = (actorX-xCoord)*(actorX-xCoord)+(actorY-yCoord)*(actorY-yCoord);
    double distance = (double) sqrt((double) distanceSquared);
    if(distance <= 12.00){
      m_actors[i]->makeSittingObjectVisible();
    }
  }
}

//Return true if an actor can move to (x, y)
//Return false otherwise
//If canMoveThroughDirt is false, then check for dirt
bool StudentWorld::actorCanMoveHere(int x, int y, bool canMoveThroughDirt) const
{
  //Check for out-of-bounds
  if(x < 0 || x > 56 || y > 60 || y < 0)
    return false;

  //If there is dirt, here, then no one can move here if canMoveThroughDirt
  //is not specified

  //Designed for a sprite of size 4
  if(!canMoveThroughDirt){
    for(int i = x; i < x + 4 && i < 60; i++){
      for(int k = y; k < y + 4 && k < 60; k++){
	if(isDirt(i, k))
	  return false;
      }
    }
  }

  //If there is, at this point, an actor that cannot be passed through,
  //then return false
  for(size_t i = 0; i < m_actors.size(); i++){
    //Ensure that actor is within radius of 3 of spot being examined
    if(withinRadiusOf(m_actors[i]->getX(), m_actors[i]->getY(), x, y, 3)){
      if(m_actors[i]->blocksMovement())
	return false;
    }
  }

  return true;
}

// Return a random int from min to max, inclusive
int StudentWorld::randInt(int min, int max) const
{
  if (max < min)
    swap(max, min);
  static random_device rd;
  static mt19937 generator(rd());
  uniform_int_distribution<> distro(min, max);
  return distro(generator);
}

//true if FrackMan is within 2x2 radius of space two spots in front of protester
bool StudentWorld::canAnnoyFrackMan(Protester* p) const
{
  int x = p->getX();
  int y = p->getY();
  p->newCoords(x, y, 2, p->getDirection());
  return (withinRadiusOf(x, y, m_frackman->getX(), m_frackman->getY(), 2));
}

//strategy: see if it is possible to leave oil field from
//square above, square left, square right, and square below
//blocking out the initial square
//store them all in bools
//try to go right first, then up, then left, then down
Actor::Direction StudentWorld::leaveFieldDirection(Actor* p) const
{
  int xStart = p->getX();
  int yStart = p->getY();

  /*            TRY GOING TO THE RIGHT              */

  int xNext = xStart;
  int yNext = yStart;

  p->newCoords(xNext, yNext, 1, Actor::right);

  if(canLeaveFieldFromHere(xStart, yStart, xNext, yNext))
    return Actor::right;


  /*              TRY GOING UP                    */

  xNext = xStart;
  yNext = yStart;

  p->newCoords(xNext, yNext, 1, Actor::up);

  if(canLeaveFieldFromHere(xStart, yStart, xNext, yNext))
     return Actor::up;

  /*               TRY GOING LEFT                */
  xNext = xStart;
  yNext = yStart;

  p->newCoords(xNext, yNext, 1, Actor::left);

  if(canLeaveFieldFromHere(xStart, yStart, xNext, yNext))
     return Actor::left;

  /*    NOW THE ONLY POSSIBLE EXIT DIRECTION IS DOWN   */
  return Actor::down;
}

//return true if there is an uninterrupted line of sight to the Frackman
//in the direction indicated from the actor
bool StudentWorld::openSightFrackMan(int x, int y, Actor::Direction dir) const
{
  //if at the coordinates of the frackman, return true
  if(x == m_frackman->getX() && y == m_frackman->getY())
    return true;

  //if coordinate is not passable, return false
  if(!actorCanMoveHere(x, y, false))
    return false;

  //recursively keep going in given direction
  if(dir  == Actor::left)
    return openSightFrackMan(x - 1, y, Actor::left);
  else if(dir == Actor::right)
    return openSightFrackMan(x + 1, y, Actor::right);
  else if(dir == Actor::down)
    return openSightFrackMan(x, y - 1, Actor::down);
  else
    return openSightFrackMan(x, y + 1, Actor::up);
}

//if oil field can be left from (xStart, yStart) without going over (xPrev, yPrev),
//return true. Otherwise, return false
bool StudentWorld::canLeaveFieldFromHere(int xPrev, int yPrev, int xStart, int yStart) const
{
  bool seenMaze[56][60];
  for(int i = 0; i < 56; i++){
    for(int k = 0; k < 60; k++){
      seenMaze[i][k] = false;
    }
  }
  
  //Cannot move back onto previous point
  seenMaze[xPrev][yPrev] = true;
  
  //Determine if it is possible to leave maze from here
  //(like stack, except instead of .top() it's .front()
  queue<Coord> myCoords;
  explore(seenMaze, myCoords, xStart, yStart);

  while( !myCoords.empty() )
    {
      Coord curr = myCoords.front();
      myCoords.pop();

      int cr = curr.r();
      int cc = curr.c();

      if(cr == 56 && cc == 60)
	return true;

      //Explore all the different directions
      explore(seenMaze, myCoords, cr-1, cc);
      explore(seenMaze, myCoords, cr+1, cc);
      explore(seenMaze, myCoords, cr, cc-1);
      explore(seenMaze, myCoords, cr, cc+1); 
    }
  return false;
}

//explore the given spot
void StudentWorld::explore(bool maze[56][60], queue<Coord>& myCoords, int r, int c) const
{
  //  enum mazeTracker {WALL, OPEN, SEEN};

  if(actorCanMoveHere(r, c, false) && !maze[r][c])
    {
      myCoords.push(Coord(r, c));
      maze[r][c] = true; //just ensure that it's not OPEN
    }
}

void StudentWorld::setDisplayText()
{
  int score = getScore();
  int level = getLevel();
  int lives = getLives();
  int health = m_frackman->getHealth();
  int squirts = m_frackman->getWater();
  int gold = m_frackman->getGold();
  int sonar = m_frackman->getSonars();
  int barrelsLeft = m_barrels;

  //Create string to display using the values
  string s = formatDisplayText(score, level, lives, health, squirts,
			       gold, sonar, barrelsLeft);

  //Set display text with newly created string
  setGameStatText(s);

  return;
}

//Return true if (x1, y1) is within a radius of rad from (x2, y2)
//Return false otherwise
bool StudentWorld::withinRadiusOf(int x1, int y1, int x2, int y2, int rad) const
{
  double distanceSquared = (double) ((x1-x2) * (x1-x2)) + ((y1-y2) * (y1-y2));
  double distance = sqrt(distanceSquared);

  if(distance <= (double) rad)
    return true;

  return false;
}

string StudentWorld::formatDisplayText(int score, int level, int lives, int health,
				       int squirts, int gold, int sonar, int barrelsLeft) const
{
  string s = "Scr: ";
  s += formatDigit(score, 6, true);

  s += " Lvl: ";
  s += formatDigit(level, 2, false);

  s += " Lives: ";
  s += formatDigit(lives, 1, true);

  //convert to percentage instead of hitpoints
  int healthPercent = health*10;
  s += " Hlth: ";
  s += formatDigit(healthPercent, 3, false);
  s+= "%";

  s += " Water:";
  s += formatDigit(squirts, 2, false);

  s += " Gld:";
  s += formatDigit(gold, 2, false);

  s += " Sonar:";
  s += formatDigit(sonar, 2, false);

  s += " Oil Left:";
  s += formatDigit(barrelsLeft, 2, false);
  
  return s;
}

//For the int input, return a string of length totalDigits that is
//that input, plus any number of leading characters necessary.
//if zeros is true, then the leading chars will be zeros, otherwise, they
//will be spaces
string StudentWorld::formatDigit(int input, int totalDigits, bool zeros) const
{
  char filler;

  //Determine what leading char should be
  if(zeros)
    filler = '0';
  else
    filler = ' ';

  //If input is zero, build and return that number of filler chars
  if(input == 0){
    string s = "0";
    for(int i = 0; i < totalDigits - 1; i++)
      s = filler + s;
    return s;
  }

  string s = to_string(input);
  for(int i = s.length(); i < totalDigits; i++){
    s = filler + s;
  }

  return s;
}

