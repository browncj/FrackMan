#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(int imageId, int startX, int startY, Direction startDirection,
	StudentWorld* world, float size, unsigned int depth);
  virtual ~Actor();
  virtual void doSomething() = 0;
  StudentWorld* getWorld(); //TODO: See if this can be made private
private:
  StudentWorld* m_world;
};

class Dirt : public Actor
{
public:
  Dirt(int startX, int startY, StudentWorld* world);
  ~Dirt();
  void doSomething();
private:
};

//TODO: This class will probably end up deriving from
//some other, more specific base class
class FrackMan : public Actor
{
public:
  FrackMan(StudentWorld* world);
  ~FrackMan();
  void doSomething();
private:
  void processMovement(Direction moveDir);
  //TODO: Figure out which ones of these actually need to be here
  int m_health;
  int m_water;
  int m_sonar;
  int m_gold;
};

#endif // ACTOR_H_
