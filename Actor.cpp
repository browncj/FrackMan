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
  : Actor(IID_PLAYER, 30, 60, right, world, 1.0, 0)
{
  m_health = 10;
  m_water = 5;
  m_sonar = 1;
  m_gold = 0;
  setVisible(true);
}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
  //TODO: Check if alive

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
	  //TODO: Fire a squirt
	  //+ other stuff, include playing sound
	  break;
	case KEY_PRESS_ESCAPE:
	  //TODO: FrackMan sets itself to dead
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

int FrackMan::getHealth(){
  return m_health;
}

int FrackMan::getWater(){
  return m_water;
}

int FrackMan::getSonars(){
  return m_sonar;
}

int FrackMan::getGold(){
  return m_gold;
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
