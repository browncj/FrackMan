#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <queue>

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
  bool isDirt(int x, int y) const;

  //returns a pointer to any actor at (x, y), otherwise returns NULL
  Actor* getActor(int x, int y);

  //gives an actor to the StudentWorld class to manage
  void giveActor(Actor* p);

  //If the FrackMan is within a radius of a, return a pointer to
  //the FrackMan, otherwise return NULL
  FrackMan* findNearbyFrackMan(Actor* a, int radius) const;

  //return a pointer to one protester within a radius of radius of actor
  //if no such protester exists, then return NULL
  //TODO: Use base protester class for differentiated protester behavior
  RegularProtester* findNearbyProtester(Actor* a, int radius) const;

  //Annoy all nearby protesters within a radius of radius by the
  //amount specified
  //return true if at least one protester is annoyed
  bool annoyNearbyProtesters(Actor* a, int radius, int amount);

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
  bool actorCanMoveHere(int x, int y, bool canMoveThroughDirt) const;

  //Return a random interger between min and max, inclusive
  int randInt(int min, int max) const;

  //Returns true if protester can annoy FrackMan
  //TODO: Use larger protester class
  bool canAnnoyFrackMan(RegularProtester* p) const;

  //Returns the direction a protester must walk to in order to leave
  //the oil field
  Actor::Direction leaveFieldDirection(Actor* p) const;

private:
  //A structure for keeping track of coordinates
  class Coord
  {
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const {return m_r;}
    int c() const {return m_c;}
  private:
    int m_r;
    int m_c;
  };
  
  //Return true if oil field can be exited from this point
  bool canLeaveFieldFromHere(int xPrev, int yPrev, int xStart, int yStart) const;
  
  //helper function for canLeaveFieldFromHere
  void explore(bool maze[56][60], std::queue<Coord>& myCoords, int r, int c) const;

  void setDisplayText();
  bool withinRadiusOf(int x1, int y1, int x2, int y2, int rad) const;
  std::string formatDisplayText(int score, int level, int lives, int health,
				int squirts, int gold, int sonar, int barrelsLeft) const;
  std::string formatDigit(int input, int totalDigits, bool zeros) const;
  std::vector<Actor*> m_actors;
  int m_barrels; //Number of barrels left on current level
  Dirt* m_dirt[60][60];
  FrackMan* m_frackman;
};

#endif // STUDENTWORLD_H_
