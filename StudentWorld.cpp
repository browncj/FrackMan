#include "StudentWorld.h"
#include <vector>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
  return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
  : GameWorld(assetDir)
{
}

int StudentWorld::init()
{
  //Initialize new data structures to keep track of game's world

  //Construct new oil field as described

  //Allocate and insert FrackMan object into game (at right location)
  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
  //Update the text with at the top with score/level/lives etc.
  setDisplayText();

  decLives();
  return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
  //Ever actor must be deleted
  //includes the FrackMan, all dirt, and everything else
  return; 
}

void StudentWorld::setDisplayText()
{
  /*
  int score;
  int level;
  int lives;
  int health;
  int squirts;
  int gold;
  int sonar;
  int barrelsLeft;

  //Create string to display using the values
  string s = formatDisplayText(score, level, lives, health, squirts,
			       gold, solar, barrelsLeft);

  //Set display text with newly created string
  setGameStatText(s);
  */
  return; //TODO: Implement
}

void formatDisplayText(int score, int level, int lives, int health, int squirts,
		       int gold, int sonar, int barrelsLeft)
{
  return; // TODO: Implement
}
