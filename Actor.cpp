#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, int startX, int startY, Direction startDirection,
	StudentWorld* world, float size, unsigned int depth)
  : GraphObject(imageId, startX, startY, startDirection, size, depth)
{
  m_world = world;
}

StudentWorld* Actor::getWorld()
{
  return m_world;
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
  //TODO: Check remove any dirt the FrackMan overlaps

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
      if(getX() != 60)
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
