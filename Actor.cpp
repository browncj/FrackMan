#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, int startX, int startY, Direction startDirection,
	     StudentWorld* world, float size, unsigned int depth)
  : GraphObject(imageId, startX, startY, startDirection, size, depth)
{
  m_world = world;
  m_alive = true;
}

StudentWorld* Actor::getWorld()
{
  return m_world;
}

bool Actor::isAlive()
{
  return m_alive;
}

void Actor::setState(bool alive)
{
  m_alive = alive;
}

Actor::~Actor()
{}

//Given an (x, y) pair and a direction, this function modifies
//x and y by moving the coordinate dist units in the direction
//of dir
void Actor::newCoords(int& x, int& y, int dist, Direction dir)
{
  if(dir == up)
    y += dist;
  else if(dir == down)
    y -= dist;
  else if(dir == left)
    x -= dist;
  else if(dir == right)
    x += dist;
}

Agent::Agent(int imageId, int startX, int startY, Direction startDirection,
	     StudentWorld* world, float size, unsigned int depth, int health)
  : Actor(imageId, startX, startY, startDirection, world, size, depth)
{
  m_health = health;
}

Agent::~Agent()
{}

int Agent::getHealth()
{
  return m_health;
}

void Agent::setHealth(int health)
{
  m_health = health;
}

Dirt::Dirt(int startX, int startY, StudentWorld* world)
  : Actor(IID_DIRT, startX, startY, right, world, 0.25, 3)
{
  setVisible(true);
}

Dirt::~Dirt()
{}

//Dirt doesn't need to do anything ever
void Dirt::doSomething()
{}

FrackMan::FrackMan(StudentWorld* world)
  : Agent(IID_PLAYER, 30, 60, right, world, 1.0, 0, 10)
{
  m_water = 5;
  m_sonar = 1;
  m_gold = 0;
  setVisible(true);
}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
  //Check if dead, if so, do nothing
  if(getHealth() <= 0)
    return;

  //Check for and remove any dirt that the FrackMan overlaps
  //Also, play the digging sound if this occurs
  bool playSound = false;
  for(int i = getX(); i < getX() + 4 && i < 60; i++){
    for(int k = getY(); k < getY() + 4 && k < 60; k++){
      if ( getWorld()->destroyDirt(i, k) )
	playSound = true;
    }
  }
  if(playSound)
    getWorld()->playSound(SOUND_DIG);

  //Check if a key is pressed; if so, act on it
  int ch;
  if(getWorld()->getKey(ch)) //note, needs access to the getKey() method in the GameWorld class
    {
      //user hit a key this tick!
      switch (ch)
	{
	case KEY_PRESS_LEFT:
	  processMovement(left);
	  break;
	case KEY_PRESS_RIGHT:
	  processMovement(right);
	  break;
	case KEY_PRESS_UP:
	  processMovement(up);
	  break;
	case KEY_PRESS_DOWN:
	  processMovement(down);
	  break;
	case KEY_PRESS_SPACE:
	  if(m_water > 0){
	    //Add squirt into world
	    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
	    m_water--;

	    //TODO: Check if too close to boulder or dirt
	    Direction myDir = getDirection();
	    Squirt* newSquirt;
	    if(myDir == up)
	      newSquirt = new Squirt(getX(), getY() + 4, myDir, getWorld());
	    else if(myDir == down)
	      newSquirt = new Squirt(getX(), getY() - 4, myDir, getWorld());
	    else if(myDir == left)
	      newSquirt = new Squirt(getX() - 4, getY(), myDir, getWorld());
	    else if(myDir == right)
	      newSquirt = new Squirt(getX() + 4, getY(), myDir, getWorld());

	    //Let StudentWorld class maange the squirt
	    getWorld()->giveActor(newSquirt);
	  }
	  break;
	case KEY_PRESS_ESCAPE:
	  //Kill the FrackMan
	  setHealth(0);
	  break;
	case KEY_PRESS_TAB:
	  //TODO: Drop a gold nugget for the protesters
	  break;
	case 'z':
	case 'Z':
	  //TODO: Use a sonar
	  break;
	}
    }
}

//If FrackMan is not facing in direction given, turn to face
//that direction. Otherwise, try to move the FrackMan one
//space in the direction he is facing.
void FrackMan::processMovement(Direction moveDir)
{
  if(getDirection() != moveDir){
    //Turn FrackMan to face in direction specified
    setDirection(moveDir);
  }
  else{
    //Try to move FrackMan one step in direction specified,
    //ensuring that he does not exit the oil field
    //TODO: Make sure FrackMan can't move near a boulder, etc.
    if(moveDir == left){
      if(getX() != 0)
	moveTo(getX() - 1, getY());
    }
    else if(moveDir == right){
      if(getX() != 56)
	moveTo(getX() + 1, getY());
    }
    else if(moveDir == up){
      if(getY() != 60)
	moveTo(getX(), getY() + 1);
    }
    else if(moveDir == down){
      if(getY() != 0)
	moveTo(getX(), getY() - 1);
    }
  }

  return;
}

int FrackMan::getWater(){
  return m_water;
}

void FrackMan::setWater(int squirts){
  m_water = squirts;
}

int FrackMan::getSonars(){
  return m_sonar;
}

int FrackMan::getGold(){
  return m_gold;
}

void FrackMan::annoyAgent(unsigned int amount)
{
  setHealth( getHealth() - amount );
}

//TODO: Implement constructor
RegularProtester::RegularProtester(StudentWorld* world)
  : Actor(IID_PROTESTER, 60, 60, left, world, 1.0, 0)
{
  //TODO: Decide how many squares to move left before possibly switching direction

  //Protester starts out not trying to leave the oil field
  m_leaveOilField = false;

  //Protester can shout immediately
  m_waitUntilShout = 0;

  curTicks = ticksToWait = std::max((unsigned int) 0, 3 - (  (getWorld()->getLevel())   /4));

  //Protesters start out visible
  setVisible(true);
}

RegularProtester::~RegularProtester()
{}

void RegularProtester::doSomething()
{
  //If the protester is dead, don't do anything
  if(!isAlive())
    return;

  //If curTicks is not zero, then the protester must wait
  if( curTicks != 0 ){
    curTicks--;
    return;
  }

  //It is now time for the protester to move
  curTicks = ticksToWait;

  //Decrement counter until protester can shout again
  if(m_waitUntilShout > 0)
    m_waitUntilShout--;

  if(m_leaveOilField){
    //Do all the stuff necessary to leave the oil field

    //Dummy implementation: Just die
    setState(false); //TODO: Change

    return;
  }
  //The protester is not currently trying to leave the oil field

  //If protester is within 4 of FrackMan, and facing him, then annoy him
  FrackMan* frackman = getWorld()->findNearbyFrackMan(this, 4);
  if(frackman != NULL && m_waitUntilShout == 0)
    {
      //TODO: Check if facing in the right direction

      //Play sound
      getWorld()->playSound(SOUND_PROTESTER_YELL);

      //Damage the FrackMan
      frackman->annoyAgent(2);

      //Do not let protester shout for 15 ticks
      m_waitUntilShout = 15;
    }

  //TODO: if regular protester is straight line of frackman, more than
  //4 units away, and isn't blocked by dirt/stones

  //Otherwise, all other options exhausted. Just keep walking

  //TONS OF OTHER WEIRD STUFF???
}

//TODO: Implement
void RegularProtester::addGold()
{
  return;
}

//TODO: Implement
void RegularProtester::annoyAgent(unsigned int amount)
{
  return;
}

Boulder::Boulder(int startX, int startY, StudentWorld* world)
  : Actor(IID_BOULDER, startX, startY, down, world, 1.0, 1)
{
  //Boulders start out visible
  setVisible(true);

  m_state = stable;
  m_wait = 30;
}

Boulder::~Boulder()
{}

void Boulder::doSomething()
{
  //Make sure boulder is still alive
  if( !isAlive() )
    return;

  if(m_state == stable){
    bool fall = true;

    //Make sure there could possibly be dirt below the boulder
    if(getX() + 3 > 59 || getY() - 1 < 0){
      fall = false;
    }
    else{
      //Check all the possible dirt spots beneath the boulder
      if(getWorld()->isDirt(getX(), getY() - 1))
	fall = false;
      if(getWorld()->isDirt(getX() + 1, getY() - 1))
	fall = false;
      if(getWorld()->isDirt(getX() + 2, getY() - 1))
	fall = false;
      if(getWorld()->isDirt(getX() + 3, getY() - 1))
	fall = false;
    }

    if(fall)
      //The boulder may fall, so set the state to waiting
      m_state = waiting;
  }
  else if(m_state == waiting){
    //Decrement wait counter
    m_wait--;

    //When time has elapsed, begin falling and play the sound
    if( m_wait <= 0 ){
      m_state = falling;
      getWorld()->playSound(SOUND_FALLING_ROCK);
    }
  }
  else if(m_state == falling){
    //The boulder must die if it is at the bottom of the oil field
    bool fall = true;
    if(getY() == 0)
      fall = false;

    //Check if boulder hits top of another boulder
    for(int i = 0; i < 4; i++){
      StudentWorld* w = getWorld();
      int yOff = 4;
      if(w->getActor(getX() + i, getY() - yOff) != NULL && w->getActor(getX() + i, getY() - yOff)->isBoulder())
	fall = false;
    }

    //Check if boulder is about to run into dirt
    if(getWorld()->isDirt(getX(), getY() - 1))
      fall = false;
    if(getWorld()->isDirt(getX() + 1, getY() - 1))
      fall = false;
    if(getWorld()->isDirt(getX() + 2, getY() - 1))
      fall = false;
    if(getWorld()->isDirt(getX() + 3, getY() - 1))
      fall = false;

    if(fall) //if the boulder must fall, then do so
      moveTo(getX(), getY() - 1);
    else //otherwise, mark the boulder for deletion
      setState(false);

    //TODO: Check if boulder comes within radius of 3 of
    //a protester or the FrackMan
  }
}

Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* world)
  :  Actor(IID_WATER_SPURT, startX, startY, dir, world, 1.0, 1)
{
  //Set the initial travel distance
  m_travelDistance = 4;

  //Must be visible at outset
  setVisible(true);
}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
  //if within 3 radius of protester, causes 2 points of annoyance and then dies

  //if has traveled full travel distance, sets its state to dead
  if (m_travelDistance == 0)
    setState(false);

  //otherwise, checks if it can move one square in facing direction
  //if there is a boulder or dirt there, it sets its location to dead

  //otherwise, the squirt moves one square forward in its currently-facing direction
  int newX = getX();
  int newY = getY();
  newCoords(newX, newY, 4, getDirection());
  moveTo(newX, newY);

  //Also decrement the travel distance counter
  m_travelDistance--;
}

WaterPool::WaterPool(int startX, int startY, StudentWorld* world)
  : Actor(IID_WATER_POOL, startX, startY, right, world, 1.0, 2)
{
  setVisible(true);
  int currLevel = getWorld()->getLevel();
  m_remainingTicks = std::max(100, 300-10*currLevel);
}

WaterPool::~WaterPool()
{}

void WaterPool::doSomething()
{
  //Must not do anything if currently dead
  if( !isAlive() )
    return;

  //Check if water pool is within a radius from the FrackMan
  Actor* FrackMan = getWorld()->findNearbyFrackMan(this, 3);
  if(FrackMan != NULL){
    //The water pool must now die
    setState(false);

    //Play sound to indicate a goodie was picked up
    getWorld()->playSound(SOUND_GOT_GOODIE);

    //Tell FrackMan that he got 5 new squirts of water
    getWorld()->giveFrackManWater(5);

    //Increase the player's score by 100 points
    getWorld()->increaseScore(100);
  }
  
  //If water pool has run out of ticks, set it to be removed
  if( m_remainingTicks <= 0)
    setState(false);

  //Decrement the counter telling how long the pool will remain
  m_remainingTicks--;
}

OilBarrel::OilBarrel(int startX, int startY, StudentWorld* world)
  : Actor(IID_BARREL, startX, startY, right, world, 1.0, 2)
{
  //Add to the counter for barrels
  getWorld()->addBarrels(1);

  //Remember that barrel is not currently visible
  isVisible = false;
}

OilBarrel::~OilBarrel()
{
  //Remove one from the counter for oil barrels
  getWorld()->addBarrels(-1);
}

void OilBarrel::doSomething()
{
  //Don't do anything if the barrel is dead
  if(!isAlive())
    return;

  //TODO: Check if barrel of oil is not currently visible
  //If FrackMan is within 4 units, away, barrel must make itself visible
  Actor* FrackMan = getWorld()->findNearbyFrackMan(this, 4);
  if(FrackMan != NULL && !isVisible){
    //Set the barrel to visible
    isVisible = true;
    setVisible(true);
    return;
  }

  //If barrel is visible and within 3 from the FrackMan, it gets picked up
  Actor* FrackMan2 = getWorld()->findNearbyFrackMan(this, 3);
  if(FrackMan2 != NULL){
    //Kill the barrel
    setState(false);

    //Play the sound
    getWorld()->playSound(SOUND_FOUND_OIL);

    //Increase player's score by 1000 points
    getWorld()->increaseScore(1000);
  }
  
}
