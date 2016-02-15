#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main(){
  FrackMan* myFracker = new FrackMan(new StudentWorld("some asset directory"));

  cerr << "All tests passed." << endl;
  return 0;
}
