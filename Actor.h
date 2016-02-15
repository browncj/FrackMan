#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
  Actor(int imageId, int startX, int startY,
	Direction startDirection, float size = 1.0, unsigned int depth = 0);
  virtual ~Actor();
  virtual void doSomething() = 0;
private:
};

class Dirt : public Actor
{
public:
  Dirt(int startX, int startY);
  ~Dirt();
  void doSomething();
private:
};

#endif // ACTOR_H_
