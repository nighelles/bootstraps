#include "boilerplate.h"

#include <iostream>

using namespace std;

extern "C" Boilerplate* create_Boilerplate()
{
  return new Boilerplate;
}

extern "C" void delete_Boilerplate( Boilerplate* bp )
{
  delete bp;
}

Boilerplate::Boilerplate() {
  ;;
}

bool Boilerplate::run() {
  cout << "Boilerplate running, iteration 2" << endl;
  char a;
  cin >> a;

  return true;
}
