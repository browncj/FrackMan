#include "StudentWorld.h"
#include <iostream> //for debugging
#include <string>
#include <algorithm> //for remove_if
#include <cmath> //for square root

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
  //Set number of barrels currently on oil field
  m_barrels = 0;

  //TODO: Add actors randomly
  //Dummy implementation: just some stuff directly
  m_actors.push_back(new Boulder(10, 20, this));
  m_actors.push_back(new Boulder(20, 30, this));
  m_actors.push_back(new Boulder(30, 40, this));
  m_actors.push_back(new Boulder(10, 50, this));
  m_actors.push_back(new WaterPool(15, 20, this));
  m_actors.push_back(new WaterPool(24, 35, this));
  m_actors.push_back(new OilBarrel(40, 45, this));
  m_actors.push_back(new OilBarrel(35, 50, this));
  m_actors.push_back(new OilBarrel(45, 55, this));

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

//Determines if pointed to Actor is dead. If so, deletes and returns true
bool isDead(Actor* p)
{
  if (!p->isAlive()){
    delete p;
    return true;
  }
  return false;
}

int StudentWorld::move()
{
  //Update the text with at the top with score/level/lives etc.
  setDisplayText();

  //Ask all actors to do something
  m_frackman->doSomething();
  for(size_t i = 0; i < m_actors.size(); i++){
    m_actors[i]->doSomething();
  }

  //TODO: Right at this point, check if the FrackMan gave up

  //Delete all actors that are dead
  m_actors.erase(remove_if(m_actors.begin(), m_actors.end(), isDead), m_actors.end());

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

bool StudentWorld::isDirt(int x, int y)
{
  for(int i = 0; i < 60; i++){
    for(int k = 0; k < 60; k++){
      //Do not check null dirt
      if(m_dirt[i][k] == NULL)
	continue;

      if(m_dirt[i][k]->getX() == x && m_dirt[i][k]->getY() == y)
	return true;
    }
  }
  return false;
}

//Returns a pointer to an actor at position (x, y)
//If there is no actor there, then return NULL
Actor* StudentWorld::getActor(int x, int y)
{
  for(size_t i = 0; i < m_actors.size(); i++){
    if(m_actors[i]->getX() == x && m_actors[i]->getY() == y){
      return m_actors[i];
    }
  }
  return NULL;
}

//Allows other classes to give actors to the StudentWorld class
//to manage
void StudentWorld::giveActor(Actor* p)
{
  m_actors.push_back(p);
}

Actor* StudentWorld::findNearbyFrackMan(Actor* a, int radius) const
{
  int thix = a->getX();
  int thiy = a->getY();

  int frackx = m_frackman->getX();
  int fracky = m_frackman->getY();

  double distanceSquared = (double) ((thix - frackx) * (thix - frackx)) + ((thiy-fracky) * (thiy - fracky));
  double distance = sqrt(distanceSquared);

  if(distance <= (double) radius)
    return m_frackman;

  return NULL;
}

void StudentWorld::giveFrackManWater(int count)
{
  int curWater = m_frackman->getWater();
  m_frackman->setWater(curWater + count);
}

int StudentWorld::addBarrels(int num)
{
  m_barrels += num;
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
  int barrelsLeft = m_barrels;

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

//For the int input, return a string of length totalDigits that is
//that input, plus any number of leading characters necessary.
//if zeros is true, then the leading chars will be zeros, otherwise, they
//will be spaces
string StudentWorld::formatDigit(int input, int totalDigits, bool zeros)
{
  char filler;

  //Determine what leading char should be
  if(zeros)
    filler = '0';
  else
    filler = ' ';

  //If input is zero, build and return that number of filler chars
  if(input == 0){
    string s = "0";
    for(int i = 0; i < totalDigits - 1; i++)
      s = filler + s;
    return s;
  }

  string s = to_string(input);
  for(int i = s.length(); i < totalDigits; i++){
    s = filler + s;
  }

  return s;
}

