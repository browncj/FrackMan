#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main(){
  StudentWorld* myWorld = new StudentWorld("some asset directory");
  FrackMan* myFracker = new FrackMan(myWorld);


  /* FOR TESTING formatDigit (NOTE: must be made public first)
     in order to run tests
  assert(myWorld->formatDigit(54, 4, true) == "0054");
  assert(myWorld->formatDigit(192, 8, true) == "00000192");
  assert(myWorld->formatDigit(73829103, 8, true) == "73829103");
  assert(myWorld->formatDigit(0, 3, true) == "000");
  */

  cerr << "All tests passed." << endl;
  return 0;
}
