#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetDir);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  //tries to destroy dirt at (x, y), returns true if successful
  bool destroyDirt(int x, int y);

  //returns true if dirt is at (x, y)
  bool isDirt(int x, int y);

  //returns a pointer to any actor at (x, y), otherwise returns NULL
  Actor* getActor(int x, int y);

  //gives an actor to the StudentWorld class to manage
  void giveActor(Actor* p);

  //If the FrackMan is within a radius of a, return a pointer to
  //the FrackMan, otherwise return NULL
  FrackMan* findNearbyFrackMan(Actor* a, int radius) const;

  //Give the FrackMan a number of squirts equal to count
  void giveFrackManWater(int count);

  //Give FrackMan a number of sonars equal to count
  void giveFrackManSonars(int count);

  //Add the specified number of barrels to the barrel counter
  int addBarrels(int num);

  //Use sonar to illuminate hidden objects within a radius of
  //12 of the FrackMan
  void useSonar(FrackMan* p);
  
  //Returns true if an actor is allowed to move to (x, y)
  //Return false otherwise
  //if canMoveThroughDirt is false, then dirt blocks the actor
  bool actorCanMoveHere(int x, int y, bool canMoveThroughDirt);

  //Return a random interger between min and max, inclusive
  int randInt(int min, int max);

private:
  void setDisplayText();
  bool withinRadiusOf(int x1, int y1, int x2, int y2, int rad);
  std::string formatDisplayText(int score, int level, int lives, int health,
				int squirts, int gold, int sonar, int barrelsLeft);
  std::string formatDigit(int input, int totalDigits, bool zeros);
  std::vector<Actor*> m_actors;
  int m_barrels; //Number of barrels left on current level
  Dirt* m_dirt[60][60];
  FrackMan* m_frackman;

};

#endif // STUDENTWORLD_H_
