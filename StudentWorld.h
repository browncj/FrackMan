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
  bool destroyDirt(int x, int y); //tries to destroy dirt at (x, y), returns true if successful
  bool isDirt(int x, int y); //returns true if dirt is at (x, y)
  Actor* getActor(int x, int y); //returns a pointer to any actor at (x, y), otherwise returns NULL
  void giveActor(Actor* p); //gives an actor to the StudentWorld class to manage
private:
  void setDisplayText();
  std::string formatDisplayText(int score, int level, int lives, int health,
				int squirts, int gold, int sonar, int barrelsLeft);
  std::string formatDigit(int input, int totalDigits, bool zeros);
  std::vector<Actor*> m_actors;
  Dirt* m_dirt[60][60];
  FrackMan* m_frackman;

};

#endif // STUDENTWORLD_H_
