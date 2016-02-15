#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetDir);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
private:
  void setDisplayText();
  std::string formatDisplayText(int score, int level, int lives, int health,
				int squirts, int gold, int sonar, int barrelsLeft);
  std::string formatDigit(int input, int totalDigits, bool zeros);
  Dirt* m_dirt[60][60];
  FrackMan* m_frackman;
};

#endif // STUDENTWORLD_H_
