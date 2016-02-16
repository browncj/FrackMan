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
{}

StudentWorld::~StudentWorld()
{
  //Just call the clean up function, which already deals
  //with any dynamically allocated memory that must be
  //handled
  cleanUp();
}

int StudentWorld::init()
{
  //TODO: Add actors

  //Add all the dirt
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      //TODO: Make sure specifying coordinates in two places
      //does not screw things up
      m_dirt[i][k] = new Dirt(i, k, this);
    }
  }
  //Delete all unnecessary dirt for initial mineshaft
  for(int i = 30; i <= 33; i++){
    for(int k = 4; k <= 59; k++){
      delete m_dirt[i][k]; 
      m_dirt[i][k] = NULL;
    }
  }

  //Allocate and insert FrackMan object into game
  m_frackman = new FrackMan(this);

  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
  //Update the text with at the top with score/level/lives etc.
  setDisplayText();

  //Ask all actors to do something
  m_frackman->doSomething();

  //TODO: Add function to remove all dead game objects

  //TODO: If player died this tick, then return and indicate so

  //TODO: Check if player has collected all barrels in level, return
  //properly if so

  return GWSTATUS_CONTINUE_GAME;
  //return GWSTATUS_FINISHED_LEVEL;
  //return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
  //Delete the FrackMan
  delete m_frackman;

  //Find all the dirt that still exists and delete it, setting
  //the pointer at that spot in the array to null
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      if(m_dirt[i][k] != NULL){
	delete m_dirt[i][k];
	m_dirt[i][k] = NULL;
      }
    }
  }

  //TODO: Delete all other actors

  return; 
}

//Searches for dirt at the location (x, y)
//if it is found, then the dirt is deleted
bool StudentWorld::destroyDirt(int x, int y)
{
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      //Do not hceck spots in the array with null pointers
      if (m_dirt[i][k] == NULL)
	continue;

      if ( m_dirt[i][k]->getX() == x && m_dirt[i][k]->getY() == y ){
	delete m_dirt[i][k];
	m_dirt[i][k] = NULL;
	return true;
      }
    }
  }
  return false;
}

void StudentWorld::setDisplayText()
{
  //TODO: Get actual values
  int score = getScore();
  int level = getLevel();
  int lives = getLives();
  int health = m_frackman->getHealth();
  int squirts = m_frackman->getWater();
  int gold = m_frackman->getGold();
  int sonar = m_frackman->getSonars();
  int barrelsLeft = 50;

  //Create string to display using the values
  string s = formatDisplayText(score, level, lives, health, squirts,
			       gold, sonar, barrelsLeft);

  //Set display text with newly created string
  setGameStatText(s);

  return;
}

//TODO: Implement (also, to use leading zeros)
string StudentWorld::formatDisplayText(int score, int level, int lives, int health,
			 int squirts, int gold, int sonar, int barrelsLeft)
{
  string s = "Scr: ";
  s += formatDigit(score, 8, true);

  s += " Lvl: ";
  s += formatDigit(level, 2, false);

  s += " Lives: ";
  s += formatDigit(lives, 1, true);

  //TODO: Display percentage, not hitpoints
  s += " Hlth: ";
  s += formatDigit(health, 3, false);

  s += " Water:";
  s += formatDigit(squirts, 2, false);

  s += " Gld:";
  s += formatDigit(gold, 2, false);

  s += " Sonar:";
  s += formatDigit(sonar, 2, false);

  s += " Oil Left:";
  s += formatDigit(barrelsLeft, 2, false);
  
  return s;
}

//TODO: Implement to correctly add leading zeros or spaces
//Current dummy implementation only adds one leading zero
//every time
string StudentWorld::formatDigit(int input, int totalDigits, bool zeros)
//if zeros is true, add leading zeros
//otherwise add leading spaces
{
  string s = to_string(input);

  string q = "0";
  q += s;
  return q;
}
