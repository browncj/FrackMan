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
  bool isAlive() const;
  void setState(bool alive);
  virtual bool isProtester() const {return false;} //returns true only if a protester
  virtual bool isBoulder() const {return false;} //Returns true only if a boulder
  StudentWorld* getWorld() const; //TODO: See if this can be made private
  virtual void makeSittingObjectVisible();
  virtual void annoyAgent(unsigned int amount) {return;} //for annoying agents
  void newCoords(int& x, int& y, int dist, Direction dir) const; //modify x and y in direction of distance

  //Returns true if other actors may pass through this one
  //By default, actors can be passed through
  virtual bool blocksMovement() const {return false;}
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
  int getHealth() const; //get health of the current Agent TODO: Make percentage??
  void setHealth(int health); //set the health of the agent
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


  int getWater() const;
  void setWater(int squirts);

  int getSonars() const;
  void setSonars(int sonars);

  int getGold() const;

  void annoyAgent(unsigned int amount); //For annoying the FrackMan
private:
  void processMovement(Direction moveDir);
  //TODO: Figure out which ones of these variables actually need to be here
  int m_water;
  int m_sonar;
  int m_gold;
};

//TODO: Factor out into larger Protester class
class RegularProtester : public Agent
{
public:
  RegularProtester(StudentWorld* world);
  ~RegularProtester();
  void doSomething();
  void addGold();
  void annoyAgent(unsigned int amount);
  bool isProtester() const {return true;} //returns true since is a protester
private:
  //Private stuff
  bool m_leaveOilField;

  int m_squaresMoveCurDirection;

  int ticksToWait; //Does not change except once every level. Ticks to wait every time
  int curTicks; //Ticks left until an action is taken. Get descremented and reset

  int m_waitUntilShout;
};

class Boulder : public Actor
{
 public:
  Boulder(int startX, int startY, StudentWorld* world);
  ~Boulder();
  void doSomething();
  virtual bool isBoulder() const {return true;} //for checking if Actor is a boulder
  bool blocksMovement() const {return true;} //boulders cannot be passed through
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

//Will describe OilBarrel, GoldNugget, SonarKit, and WaterPool
class SittingObject : public Actor
{
public:

  SittingObject(int imageId, int startX, int startY, Direction startDirection,
		StudentWorld* world, float size, unsigned int depth);
  ~SittingObject();

  //Sets object visible while also keeping track that it is now visible
  void makeSittingObjectVisible();

  //Returns true if object is currently visible
  bool isVisible() const;

  //Return number of ticks left
  int ticksLeft() const;

  //Set ticks left to exist
  void setTicks(int number);
private:
  bool m_visible; //keeps track of if object has been made visible yet
  
  //Keeps track of how much longer object will remain on the field
  //Is set to -1 if item is to remain forever
  int m_remainingTicks;
};

class WaterPool : public SittingObject
{
public:
  WaterPool(int startX, int startY, StudentWorld* world);
  ~WaterPool();
  void doSomething();
private:
};

class OilBarrel : public SittingObject
{
 public:
  OilBarrel(int startX, int startY, StudentWorld* world);
  ~OilBarrel();
  void doSomething();
 private:
};

class SonarKit : public SittingObject
{
public:
  SonarKit(int startX, int startY, StudentWorld* world);
  ~SonarKit();
  void doSomething();
private:
};

#endif // ACTOR_H_
