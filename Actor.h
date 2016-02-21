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
  virtual bool isBoulder() {return false;} //Returns true only if a boulder
  StudentWorld* getWorld(); //TODO: See if this can be made private
  void newCoords(int& x, int& y, int dist, Direction dir); //modify x and y in direction of distance

private:
  StudentWorld* m_world;
  bool m_alive;
};

//For the FrackMan and protesters
//Can be annoyed
class Agent : public Actor
{
 public:
  Agent(int imageId, int startX, int startY, Direction startDirection,
	StudentWorld* world, float size, unsigned int depth, int health);
  ~Agent();
  //Right now, I'm not overriding void doSomething() here just to see what happens
  virtual void annoyAgent() = 0; //annoy the current Agent
  int getHealth(); //get health of the current Agent TODO: Make percentage??
 private:
  int m_health;
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
class FrackMan : public Agent
{
public:
  FrackMan(StudentWorld* world);
  ~FrackMan();
  void doSomething();


  int getWater();
  void setWater(int squirts);

  int getSonars();
  int getGold();

  void annoyAgent(); //For annoying the FrackMan
private:
  void processMovement(Direction moveDir);
  //TODO: Figure out which ones of these variables actually need to be here
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
  virtual bool isBoulder() {return true;} //for checking if Actor is a boulder
 private:
  //Enumerate the three possible states of a boulder
  enum boulderState {stable, waiting, falling};
  boulderState m_state;
  int m_wait; //keeps track of ticks in waiting state
};

class Squirt : public Actor
{
 public:
  Squirt(int startX, int startY, Direction dir, StudentWorld* world);
  ~Squirt();
  void doSomething();
 private:
  int m_travelDistance;
};

class WaterPool : public Actor
{
public:
  WaterPool(int startX, int startY, StudentWorld* world);
  ~WaterPool();
  void doSomething();
private:
  int m_remainingTicks;
};

class OilBarrel : public Actor
{
 public:
  OilBarrel(int startX, int startY, StudentWorld* world);
  ~OilBarrel();
  void doSomething();
 private:
  bool isVisible; //keeps track of whether the barrel has been set to visible yet
};

#endif // ACTOR_H_
