#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, int startX, int startY, Direction startDirection,
      float size, unsigned int depth)
  : GraphObject(imageId, startX, startY, startDirection, size, depth)
{
}

Actor::~Actor()
{}

Dirt::Dirt(int startX, int startY)
  : Actor(IID_DIRT, startX, startY, right, 0.25, 3)
{
  setVisible(true);
}

Dirt::~Dirt()
{}

//Dirt doesn't need to do anything ever
void Dirt::doSomething()
{}
