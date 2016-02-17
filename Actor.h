#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

//All classes inherit from this
class Actor : public GraphObject
{
public:
  Actor(int imageId, int startX, int startY, Direction startDirection,
	StudentWorld* world, float size, unsigned int depth);
  virtual ~Actor();
  virtual void doSomething() = 0;
  bool isAlive();
  void setState(bool alive);
  StudentWorld* getWorld(); //TODO: See if this can be made private
private:
  StudentWorld* m_world;
  bool m_alive;
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
  int getHealth();
  int getWater();
  int getSonars();
  int getGold();
private:
  void processMovement(Direction moveDir);
  //TODO: Figure out which ones of these variables actually need to be here
  int m_health;
  int m_water;
  int m_sonar;
  int m_gold;
};

class Boulder : public Actor
{
 public:
  Boulder(int startX, int startY, StudentWorld* world);
  ~Boulder();
  void doSomething();
 private:
  //Enumerate the three possible states of a boulder
  enum boulderState {stable, waiting, falling};
  boulderState m_state;
  int m_wait; //keeps track of ticks in waiting state
};

#endif // ACTOR_H_
