#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, int startX, int startY, Direction startDirection,
	     StudentWorld* world, float size, unsigned int depth)
  : GraphObject(imageId, startX, startY, startDirection, size, depth)
{
  m_world = world;
  m_alive = true;
}

StudentWorld* Actor::getWorld() const
{
  return m_world;
}

void Actor::makeSittingObjectVisible()
{}

bool Actor::isAlive() const
{
  return m_alive;
}

void Actor::setState(bool alive)
{
  m_alive = alive;
}

Actor::~Actor()
{}

//Given an (x, y) pair and a direction, this function modifies
//x and y by moving the coordinate dist units in the direction
//of dir
void Actor::newCoords(int& x, int& y, int dist, Direction dir) const
{
  if(dir == up)
    y += dist;
  else if(dir == down)
    y -= dist;
  else if(dir == left)
    x -= dist;
  else if(dir == right)
    x += dist;
}

Agent::Agent(int imageId, int startX, int startY, Direction startDirection,
	     StudentWorld* world, float size, unsigned int depth, int health)
  : Actor(imageId, startX, startY, startDirection, world, size, depth)
{
  m_health = health;
}

Agent::~Agent()
{}

int Agent::getHealth() const
{
  return m_health;
}

void Agent::setHealth(int health)
{
  m_health = health;
}

Dirt::Dirt(int startX, int startY, StudentWorld* world)
  : Actor(IID_DIRT, startX, startY, right, world, 0.25, 3)
{
  setVisible(true);
}

Dirt::~Dirt()
{}

//Dirt doesn't need to do anything ever
void Dirt::doSomething()
{}

FrackMan::FrackMan(StudentWorld* world)
  : Agent(IID_PLAYER, 30, 60, right, world, 1.0, 0, 10)
{
  m_water = 5;
  m_sonar = 1;
  m_gold = 0;
  setVisible(true);
}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
  //Check if dead, if so, do nothing
  if(getHealth() <= 0)
    return;

  //Check for and remove any dirt that the FrackMan overlaps
  //Also, play the digging sound if this occurs
  bool playSound = false;
  for(int i = getX(); i < getX() + 4 && i < 60; i++){
    for(int k = getY(); k < getY() + 4 && k < 60; k++){
      if ( getWorld()->destroyDirt(i, k) )
	playSound = true;
    }
  }
  if(playSound)
    getWorld()->playSound(SOUND_DIG);

  //Check if a key is pressed; if so, act on it
  int ch;
  if(getWorld()->getKey(ch)) //note, needs access to the getKey() method in the GameWorld class
    {
      //user hit a key this tick!
      switch (ch)
	{
	case KEY_PRESS_LEFT:
	  processMovement(left);
	  break;
	case KEY_PRESS_RIGHT:
	  processMovement(right);
	  break;
	case KEY_PRESS_UP:
	  processMovement(up);
	  break;
	case KEY_PRESS_DOWN:
	  processMovement(down);
	  break;
	case KEY_PRESS_SPACE:
	  if(m_water > 0){
	    //Add squirt into world
	    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
	    m_water--;

	    Direction myDir = getDirection();
	    Squirt* newSquirt;
	    if(myDir == up)
	      newSquirt = new Squirt(getX(), getY() + 4, myDir, getWorld());
	    else if(myDir == down)
	      newSquirt = new Squirt(getX(), getY() - 4, myDir, getWorld());
	    else if(myDir == left)
	      newSquirt = new Squirt(getX() - 4, getY(), myDir, getWorld());
	    else if(myDir == right)
	      newSquirt = new Squirt(getX() + 4, getY(), myDir, getWorld());

	    //Let StudentWorld class manage the squirt
	    getWorld()->giveActor(newSquirt);
	  }
	  break;
	case KEY_PRESS_ESCAPE:
	  //Kill the FrackMan
	  setHealth(0);
	  break;
	case KEY_PRESS_TAB:
	  //Drop a gold nugget to bribe protesters if one is available
	  if(m_gold > 0){
	    m_gold--;
	    GoldNugget* newNugget = new GoldNugget(getX(), getY(), getWorld(), true, false, false);

	    //Let the StudentWorld class handle the new nugget
	    getWorld()->giveActor(newNugget);
	  }
	  break;
	case 'z':
	case 'Z':
	  if(m_sonar > 0){
	    //Decrement sonar counter
	    m_sonar--;

	    //Play sonar sound
	    getWorld()->playSound(SOUND_SONAR);

	    //Reveal nearby elements
	    getWorld()->useSonar(this);
	  }
	  break;
	}
    }
}

//If FrackMan is not facing in direction given, turn to face
//that direction. Otherwise, try to move the FrackMan one
//space in the direction he is facing.
void FrackMan::processMovement(Direction moveDir)
{
  if(getDirection() != moveDir){
    //Turn FrackMan to face in direction specified
    setDirection(moveDir);
  }
  else{
    //Try to move FrackMan one step in direction specified,
    //ensuring that he does not exit the oil field
    if(moveDir == left){
      if(getWorld()->actorCanMoveHere(getX()-1, getY(), true))
	moveTo(getX() - 1, getY());
    }
    else if(moveDir == right){
      if(getWorld()->actorCanMoveHere(getX()+1, getY(), true))//if(getX() != 56)
	moveTo(getX() + 1, getY());
    }
    else if(moveDir == up){
      if(getWorld()->actorCanMoveHere(getX(),getY()+1, true))
	moveTo(getX(), getY() + 1);
    }
    else if(moveDir == down){
      if(getWorld()->actorCanMoveHere(getX(),getY()-1, true))//if(getY() != 0)
	moveTo(getX(), getY() - 1);
    }
  }

  return;
}

int FrackMan::getWater() const
{
  return m_water;
}

void FrackMan::setWater(int squirts){
  m_water = squirts;
}

int FrackMan::getSonars() const
{
  return m_sonar;
}

void FrackMan::setSonars(int sonars){
  m_sonar = sonars;
}

int FrackMan::getGold() const
{
  return m_gold;
}

void FrackMan::annoyAgent(unsigned int amount)
{
  setHealth( getHealth() - amount );
}

Protester::Protester(StudentWorld* world, int imageID, int initialHP)
  : Agent(imageID, 60, 60, left, world, 1.0, 0, initialHP)
{
  //Protesters start out visible
  setVisible(true);

  //Protester starts out not trying to leave oil field
  m_leaveOilField = false;

  //randomly set number of squares protester walks in current direction
  setSquaresMove(getWorld()->randInt(8, 60));
  
  //set the number of ticks between actions for the protester
  curTicks = ticksToWait = std::max((unsigned int) 0, 3 - (  (getWorld()->getLevel())   /4));

  //Protester can shout right away
  m_waitUntilShout = 0;

  //Protester must wait 200 ticks until it can turn perpendicularly
  m_ticksTillNextPerp = 200;
}

Protester::~Protester()
{}

void Protester::doSomething()
{
  //If the protester is dead, don't do anything
  if(!isAlive())
    return;

  //if the protester is staring at some gold, don't do anything
  if(!stareAtGold())
    return;

  //If curTicks is not zero, then the protester must wait
  if( getCurTicks() != 0 ){
    setCurTicks(getCurTicks() - 1);
    return;
  }

  //It is now time for the protester to move
  setCurTicks(getTicksToWait());

  //Decrement counter until protester can shout again
  if(m_waitUntilShout > 0)
    m_waitUntilShout--;

  if(leavingOilField()){
    //if at exit point, then set protester to dead
    if((getX() == 60 || getX() == 56) && getY() == 60)
      setState(false);

    //get coordinates in direction protester must go in to leave
    int x = getX();
    int y = getY();
    Direction dir = getWorld()->leaveFieldDirection(this);
    newCoords(x, y, 1, dir);
    setDirection(dir);

    //now move in that direction
    moveTo(x, y);

    return;
  }
  //The protester is not currently trying to leave the oil field

  //Decrement counter for number of ticks until next perpendicular turn
  if(m_ticksTillNextPerp > 0)
    m_ticksTillNextPerp--;

  //If protester is within 4 of FrackMan, and facing him, then annoy him
  FrackMan* frackman = getWorld()->findNearbyFrackMan(this, 4);
  if(frackman != NULL && m_waitUntilShout == 0 && getWorld()->canAnnoyFrackMan(this))
    {
      //Play sound
      getWorld()->playSound(SOUND_PROTESTER_YELL);

      //Damage the FrackMan
      frackman->annoyAgent(2);
	
      //Do not let protester shout for 15 ticks
      m_waitUntilShout = 15;
	
      //Do not do anything else this tick
      return;
    }

  //if regular protester is straight line of frackman, more than
  //4 units away, and isn't blocked by dirt/stones, then face that
  //direction and take a step toward the frackman
  if(getWorld()->openSightFrackMan(getX(), getY(), left)){
    setDirection(left);
    int x = getX();
    int y = getY();
    newCoords(x, y, 1, left);
    moveTo(x, y);
    setSquaresMove(0);
    return;
  }
  else if(getWorld()->openSightFrackMan(getX(), getY(), right)){
    setDirection(right);
    int x = getX();
    int y = getY();
    newCoords(x, y, 1, right);
    moveTo(x, y);
    setSquaresMove(0);
    return;
  }
  else if(getWorld()->openSightFrackMan(getX(), getY(), up)){
    setDirection(up);
    int x = getX();
    int y = getY();
    newCoords(x, y, 1, up);
    moveTo(x, y);
    setSquaresMove(0);
    return;
  }
  else if(getWorld()->openSightFrackMan(getX(), getY(), down)){
    setDirection(down);
    int x = getX();
    int y = getY();
    newCoords(x, y, 1, down);
    moveTo(x, y);
    setSquaresMove(0);
    return;
  }

  //The protester cannot directly see the FrackMan
  setSquaresMove( getSquaresMove() - 1 );

  if(getSquaresMove() <= 0){

    //Infinite loop that will be broken out of as soon as a direction is
    //successfully chosen
    while(true){
      //At this point, change directions
      Direction newDir = (Direction)  getWorld()->randInt(1, 4);

      int newX = getX();
      int newY = getY();
    
      newCoords(newX, newY, 1, newDir);

      if(getWorld()->actorCanMoveHere(newX, newY, false)){
	setDirection(newDir);
	setSquaresMove(getWorld()->randInt(8, 60));

	//Leave outer while loop
	goto OUTOFLOOP;
      }
      //Do not return out of function, continue on with next step
    }
  OUTOFLOOP:
    ;
  }

  //determine if the protester could move in certain perpendicular directions
  if((getDirection() == left || getDirection() == right) && m_ticksTillNextPerp == 0){
    bool b_up = false;
    bool b_down = false;

    int x = getX();
    int y = getY();

    newCoords(x, y, 1, up);
    if(getWorld()->actorCanMoveHere(x, y, false))
      b_up = true;

    x = getX();
    y = getY();
    newCoords(x, y, 1, down);
    if(getWorld()->actorCanMoveHere(x, y, false))
      b_down = true;

    if(b_up && !b_down){
      //Turn up
      setDirection(up);
      setSquaresMove(getWorld()->randInt(8, 60));
      m_ticksTillNextPerp = 200;
    }
    else if(!b_up && b_down){
      setDirection(down);
      setSquaresMove(getWorld()->randInt(8, 60));
      m_ticksTillNextPerp = 200;
    }
    else if(b_up && b_down){
      int randomNum = getWorld()->randInt(0, 1);
      if(randomNum == 0){
	setDirection(up);
	setSquaresMove(getWorld()->randInt(8, 60));
	m_ticksTillNextPerp = 200;
      }
      else{
	setDirection(down);
	setSquaresMove(getWorld()->randInt(8, 60));
	m_ticksTillNextPerp = 200;
      }
    }
  }
  else if((getDirection() == up || getDirection() == down) && m_ticksTillNextPerp == 0){
    bool b_left = false;
    bool b_right = false;

    int x = getX();
    int y = getY();

    newCoords(x, y, 1, left);
    if(getWorld()->actorCanMoveHere(x, y, false))
      b_left = true;

    x = getX();
    y = getY();
    newCoords(x, y, 1, right);
    if(getWorld()->actorCanMoveHere(x, y, false))
      b_right = true;

    if(b_left && !b_right){
      //Turn up
      setDirection(left);
      setSquaresMove(getWorld()->randInt(8, 60));
      m_ticksTillNextPerp = 200;
    }
    else if(!b_left && b_right){
      setDirection(right);
      setSquaresMove(getWorld()->randInt(8, 60));
      m_ticksTillNextPerp = 200;
    }
    else if(b_left && b_right){
      int randomNum = getWorld()->randInt(0, 1);
      if(randomNum == 0){
	setDirection(left);
	setSquaresMove(getWorld()->randInt(8, 60));
	m_ticksTillNextPerp = 200;
      }
      else{
	setDirection(right);
	setSquaresMove(getWorld()->randInt(8, 60));
	m_ticksTillNextPerp = 200;
      }
    }
  }


  //Try to take one step in direction that is being faced
  int newX = getX();
  int newY = getY();
  newCoords(newX, newY, 1, getDirection());

  //If protester is starting out, allow it to move onto oil field
  if(getX() > 56 && getDirection() == left)
    moveTo(newX, newY);
  //Else check for new position being valid
  else if(getWorld()->actorCanMoveHere(newX, newY, false))
    moveTo(newX, newY);
  else
    //Protester cannot move, so take no more steps in this direction
    setSquaresMove(0);
}

void Protester::annoyAgent(unsigned int amount)
{
  //Protester cannot be annoyed if it is leaving the oil field
  if (leavingOilField())
    return;

  //Subtract the damage from the protester's health
  setHealth( getHealth() - amount );

  //Play sound if protester is still alive
  if(getHealth() > 0){
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);

    unsigned int maxPartner = 50;

    //Extend the resting state of the protester
    setCurTicks(std::max(maxPartner, 100-(getWorld()->getLevel() * 10 )));
  }
  else if(getHealth() <= 0){
    //The protester will now try to leave the oil field
    setLeaveField();
    getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);

    //Do something on the very next tick
    setCurTicks(0);

    //Check if protester was killed by squirt
    if(amount == 2){
      //add 100 points
      addPointsDueToSquirtKill();
    }
    else if(amount == 100){ //check if killed by boulder
      //add 500 points
      getWorld()->increaseScore(500);
    }
    
  }
}

RegularProtester::RegularProtester(StudentWorld* world)
  : Protester(world, IID_PROTESTER, 5)
{
}

RegularProtester::~RegularProtester()
{}

void RegularProtester::addGold()
{
  //play the "I'm rich!" sound effect
  getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);

  //note: gold nugget class handled increasing score for bribery
  
  //the protester now decides to leave the oil field
  setLeaveField();
}

void RegularProtester::addPointsDueToSquirtKill(){
  getWorld()->increaseScore(100);
}


HardCoreProtester::HardCoreProtester(StudentWorld* world)
  : Protester(world, IID_HARD_CORE_PROTESTER, 20)
{
  //hardcore protesters do not start staring at gold
  m_stareAtGold = 0;
}

HardCoreProtester::~HardCoreProtester()
{}

//bribery for hardcore protesters
void HardCoreProtester::addGold()
{
  //play entertaining sound for the player
  getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);

  //player gets points for the bribery
  //this is an addition to the 25 points that the nugget class provides
  getWorld()->increaseScore(25);

  //set number of ticks hardcore protester will stare
  m_stareAtGold = std::max((unsigned int) 50, 100 - getWorld()->getLevel() * 10);
}

bool HardCoreProtester::stareAtGold()
{
  if(m_stareAtGold <= 0)
    return true;
  else{
    m_stareAtGold--;
    return false;
  }
}

void HardCoreProtester::addPointsDueToSquirtKill(){
  getWorld()->increaseScore(250);
}

Boulder::Boulder(int startX, int startY, StudentWorld* world)
  : Actor(IID_BOULDER, startX, startY, down, world, 1.0, 1)
{
  //Boulders start out visible
  setVisible(true);

  m_state = stable;
  m_wait = 30;
}

Boulder::~Boulder()
{}

void Boulder::doSomething()
{
  //Make sure boulder is still alive
  if( !isAlive() )
    return;

  if(m_state == stable){
    bool fall = true;

    //Make sure there could possibly be dirt below the boulder
    if(getX() + 3 > 59 || getY() - 1 < 0){
      fall = false;
    }
    else{
      //Check all the possible dirt spots beneath the boulder
      if(getWorld()->isDirt(getX(), getY() - 1))
	fall = false;
      if(getWorld()->isDirt(getX() + 1, getY() - 1))
	fall = false;
      if(getWorld()->isDirt(getX() + 2, getY() - 1))
	fall = false;
      if(getWorld()->isDirt(getX() + 3, getY() - 1))
	fall = false;
    }

    if(fall)
      //The boulder may fall, so set the state to waiting
      m_state = waiting;
  }
  else if(m_state == waiting){
    //Decrement wait counter
    m_wait--;

    //When time has elapsed, begin falling and play the sound
    if( m_wait <= 0 ){
      m_state = falling;
      getWorld()->playSound(SOUND_FALLING_ROCK);
    }
  }
  else if(m_state == falling){
    //Check if FrackMan is nearby; if so, annoy him
    FrackMan* f = getWorld()->findNearbyFrackMan(this, 3);
    if(f != NULL){
      f->annoyAgent(100);
    }

    //Annoy any protesters that are too close
    getWorld()->annoyNearbyProtesters(this, 3, 100);

    //The boulder must die if it is at the bottom of the oil field
    bool fall = true;
    if(getY() == 0)
      fall = false;

    //Check if boulder hits top of another boulder
    for(int i = 0; i < 4; i++){
      StudentWorld* w = getWorld();
      int yOff = 4;
      if(w->getActor(getX() + i, getY() - yOff) != NULL && w->getActor(getX() + i, getY() - yOff)->isBoulder())
	fall = false;
    }

    //Check if boulder is about to run into dirt
    if(getWorld()->isDirt(getX(), getY() - 1))
      fall = false;
    if(getWorld()->isDirt(getX() + 1, getY() - 1))
      fall = false;
    if(getWorld()->isDirt(getX() + 2, getY() - 1))
      fall = false;
    if(getWorld()->isDirt(getX() + 3, getY() - 1))
      fall = false;

    if(fall) //if the boulder must fall, then do so
      moveTo(getX(), getY() - 1);
    else //otherwise, mark the boulder for deletion
      setState(false);
  }
}

Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* world)
  :  Actor(IID_WATER_SPURT, startX, startY, dir, world, 1.0, 1)
{
  //Set the initial travel distance
  m_travelDistance = 4;

  //Must be visible at outset
  setVisible(true);
}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
  //if within 3 radius of protester, causes 2 points of annoyance and then dies
  if( getWorld()->annoyNearbyProtesters(this, 3, 2) )
    setState(false);

  //if has traveled full travel distance, sets its state to dead
  if (m_travelDistance == 0)
    setState(false);

  //otherwise, checks if it can move one square in facing direction
  //if there is a boulder or dirt there, it sets its location to dead
  int newX = getX();
  int newY = getY();
  newCoords(newX, newY, 4, getDirection());
  if( !getWorld()->actorCanMoveHere(newX, newY, false) ){
    setState(false);
  }

  //otherwise, the squirt moves one square forward in its currently-facing direction
  moveTo(newX, newY);

  //Also decrement the travel distance counter
  m_travelDistance--;
}

SittingObject::SittingObject(int imageId, int startX, int startY, Direction startDirection,
			     StudentWorld* world, float size, unsigned int depth)
  : Actor(imageId, startX, startY, startDirection, world, size, depth)
{
  //By default, ignore the tick countdown
  m_remainingTicks = -1;

  //Assume that object does not start out visible
  m_visible = false;
}

SittingObject::~SittingObject()
{}

void SittingObject::makeSittingObjectVisible()
{
  //Make the object visible
  setVisible(true);

  //Keep track of the visibility
  m_visible = true;
}

bool SittingObject::isVisible() const
{
  return m_visible;
}

int SittingObject::ticksLeft() const
{
  return m_remainingTicks;
}

void SittingObject::setTicks(int number)
{
  m_remainingTicks = number;
}

WaterPool::WaterPool(int startX, int startY, StudentWorld* world)
  : SittingObject(IID_WATER_POOL, startX, startY, right, world, 1.0, 2)
{
  //All water pools start off visible
  makeSittingObjectVisible();

  //Set number of ticks to remain based off of level
  int currLevel = getWorld()->getLevel();
  setTicks(std::max(100, 300-10*currLevel));
}

WaterPool::~WaterPool()
{}

void WaterPool::doSomething()
{
  //Must not do anything if currently dead
  if( !isAlive() )
    return;

  //Check if water pool is within a radius from the FrackMan
  Actor* FrackMan = getWorld()->findNearbyFrackMan(this, 3);
  if(FrackMan != NULL){
    //The water pool must now die
    setState(false);

    //Play sound to indicate a goodie was picked up
    getWorld()->playSound(SOUND_GOT_GOODIE);

    //Tell world to give FrackMan 5 squirts of water
    getWorld()->giveFrackManWater(5);

    //Increase the player's score by 100 points
    getWorld()->increaseScore(100);
  }
  
  //If water pool has run out of ticks, set it to be removed
  if( ticksLeft() == 0)
    setState(false);

  //Decrement the counter telling how long the pool will remain
  //if a positive number of ticks remain
  if(ticksLeft() > 0)
    setTicks(ticksLeft() - 1);
}

OilBarrel::OilBarrel(int startX, int startY, StudentWorld* world)
  : SittingObject(IID_BARREL, startX, startY, right, world, 1.0, 2)
{
  //Add to the counter for barrels
  getWorld()->addBarrels(1);
}

OilBarrel::~OilBarrel()
{
  //Remove one from the counter for oil barrels
  getWorld()->addBarrels(-1);
}

void OilBarrel::doSomething()
{
  //Don't do anything if the barrel is dead
  if(!isAlive())
    return;

  //Check if barrel of oil is not currently visible
  //If FrackMan is within 4 units, away, barrel must make itself visible
  Actor* FrackMan = getWorld()->findNearbyFrackMan(this, 4);
  if(FrackMan != NULL && !isVisible()){
    //Set the barrel to visible
    makeSittingObjectVisible();
    return;
  }

  //If barrel is visible and within 3 from the FrackMan, it gets picked up
  Actor* FrackMan2 = getWorld()->findNearbyFrackMan(this, 3);
  if(FrackMan2 != NULL){
    //Kill the barrel
    setState(false);

    //Play the sound
    getWorld()->playSound(SOUND_FOUND_OIL);

    //Increase player's score by 1000 points
    getWorld()->increaseScore(1000);
  }
  
}


SonarKit::SonarKit(int startX, int startY, StudentWorld* world)
  : SittingObject(IID_SONAR, startX, startY, right, world, 1.0, 2)
{
  //All sonar kits start out visible
  makeSittingObjectVisible();

  int currLevel = getWorld()->getLevel();
  setTicks(std::max(100, 300-10*currLevel));
}

SonarKit::~SonarKit()
{}

void SonarKit::doSomething()
{
  //Do nothing if not currently active
  if(!isAlive())
    return;

  //Check if within a radius of 3 of the FrackMan
  Actor* FrackMan = getWorld()->findNearbyFrackMan(this, 3);
  if(FrackMan != NULL){
    //Water pool is dead
    setState(false);

    //Play sound
    getWorld()->playSound(SOUND_GOT_GOODIE);

    //Tell world to give FrackMan a sonar
    getWorld()->giveFrackManSonars(1);

    //Increase the score by 75 points
    getWorld()->increaseScore(75);
  }

  //Check if tick lifetime has elapsed
  if( ticksLeft() == 0)
    setState(false);

  if(ticksLeft() > 0)
    setTicks(ticksLeft() - 1);
}

GoldNugget::GoldNugget(int startX, int startY, StudentWorld* world, bool visible,
		       bool forFrackMan, bool permanent)
  : SittingObject(IID_GOLD, startX, startY, right, world, 1.0, 2)
{
  if(visible)
    makeSittingObjectVisible();

  m_forFrackMan = forFrackMan;
  m_permanent = permanent;
  m_ticksLeft = 100;
}

GoldNugget::~GoldNugget()
{}

void GoldNugget::doSomething()
{
  //do not do something if nugget is not alive
  if(!isAlive())
    return;

  //if nugget invisible and FrackMan <= 4.00 units away, make visible
  FrackMan* frackman = getWorld()->findNearbyFrackMan(this, 4);
  if(frackman != NULL && !isVisible() ){
    makeSittingObjectVisible();
    return;
  }

  //if nugget is pickuppable by frackman and under 3.0 units away,
  //then nugget will activate
  frackman = getWorld()->findNearbyFrackMan(this, 3);
  if(frackman != NULL && m_forFrackMan){
    //nugget must die
    setState(false);

    //play happy sound to encourage the FrackMan
    getWorld()->playSound(SOUND_GOT_GOODIE);

    //increase score by 10 points
    getWorld()->increaseScore(10);

    //Tell FrackMan that he got a new nugget
    frackman->setGold( frackman->getGold() + 1 );
  }

  //Check if protester is nearby for bribing
  Protester* pro = getWorld()->findNearbyProtester(this, 3);
  if(pro != NULL && !m_forFrackMan){
    //Nugget must now die
    setState(false);

    //Play sound so player knows protester got bribed
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);

    //Tell protester that it just got bribed
    pro->addGold();

    //Increase score by 25 points
    getWorld()->increaseScore(25);
    
  }

  //If nugget is temporary, then check if its tick lifetime
  //has elapsed and set state to dead
  if(!m_permanent){
    m_ticksLeft--;
    if(m_ticksLeft <= 0)
      setState(false);
  }
}
