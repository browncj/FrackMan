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
  int setGold(int golds) {m_gold = golds;}

  void annoyAgent(unsigned int amount); //For annoying the FrackMan
private:
  void processMovement(Direction moveDir);
  //TODO: Figure out which ones of these variables actually need to be here
  int m_water;
  int m_sonar;
  int m_gold;
};

class Protester : public Agent
{
 public:
  Protester(StudentWorld* world, int imageID, int initialHP);
  ~Protester();
  void doSomething();

  //for bribing the given protester
  virtual void addGold() = 0;

  //returns true if protester is trying to leave oil field
  bool leavingOilField() const {return m_leaveOilField;}

  //set that the protester must now leave the oil field
  void setLeaveField() {m_leaveOilField = true;}

  //set number of squares to move to the number specified
  void setSquaresMove(int squares) {m_squaresMoveCurDirection = squares;}

  //get the number of squares left for protester to move in current direction
  int getSquaresMove() {return m_squaresMoveCurDirection;}

  //get current ticks that protester should wait
  int getCurTicks() const {return curTicks;}

  //set the number of ticks protester has left to wait at the moment
  void setCurTicks(int ticks) {curTicks = ticks;}

  //get ticks that protester must wait every cycle for this level
  int getTicksToWait() const {return ticksToWait;}

  bool isProtester() const {return true;} //returns true since is a protester

  //used to annoy the protester
  void annoyAgent(unsigned int amount);
  
 private:
  //for increasing score due to a squirt kill
  virtual void addPointsDueToSquirtKill() = 0;

  int m_squaresMoveCurDirection; //number of ticks to walk in current direction
  bool m_leaveOilField; //true if protester is trying to leave oil field

  int ticksToWait; //Does not change except once every level. Ticks to wait every time
  int curTicks; //Ticks left until an action is taken. Get descremented and reset

  int m_waitUntilShout; //ticks to wait until protester can shout again

  int m_ticksTillNextPerp; //ticks to wait until protester can make a perpendicular turn again
};

class RegularProtester : public Protester
{
public:
  RegularProtester(StudentWorld* world);
  ~RegularProtester();
  virtual void addGold(); //for bribing the protester
private:
  virtual void addPointsDueToSquirtKill();
};

class HardCoreProtester : public Protester
{
 public:
  HardCoreProtester(StudentWorld* world);
  ~HardCoreProtester();
  virtual void addGold(); //for bribing the hardcore protester
 private:
  virtual void addPointsDueToSquirtKill();
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

class GoldNugget : public SittingObject
{
 public:
  //forFrackMan is true if the nugget is pickuppable by FrackMan, false if
  //it's pickuppable by protesters
  GoldNugget(int startX, int startY, StudentWorld* world, bool visible,
	     bool forFrackMan, bool permanent);
  ~GoldNugget();
  void doSomething();
 private:
  bool m_forFrackMan; //true if takable by FrackMan; false if takable by protesters
  bool m_permanent; //true if nugget is permanent; false if nugget is temporary
  int m_ticksLeft; //counts number of ticks nugget will remain if in temporary state (100)
};

#endif // ACTOR_H_
