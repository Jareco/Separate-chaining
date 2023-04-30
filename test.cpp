#include <iostream>
#include "ADS_set.h"
using namespace std;

int main(){
  cout << "Testing program for ADS_Set" << endl;

  ADS_set<int> set1={1,5,34,34,54};
  set1.dump();

}


